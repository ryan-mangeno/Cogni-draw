from openai import OpenAI
from dotenv import load_dotenv
import os
import requests
import base64

load_dotenv()
api_key = os.getenv("API_KEY")
client = OpenAI(api_key=api_key)


def encode_image_as_data_url(image_path: str) -> str:
    with open(image_path, "rb") as img:
        b64 = base64.b64encode(img.read()).decode("utf-8")
    return f"data:image/png;base64,{b64}"

image_path = "../Resources/Drawings/drawing.png"
data_url = encode_image_as_data_url(image_path)


with open(image_path, 'rb') as image_file:
    upload_response = client.files.create(file=image_file, purpose='vision')

file_id = upload_response.id

first_prompt = """
Determine what this input content is and write a short description of it to then prompt DALL·E 2.0 to make it into a 3D model. 
Note: it is a 2D drawing sketch, and in the resulting prompt, make sure to keep the original style of the drawing, with a black background.
"""

response = client.chat.completions.create(
    model="gpt-4o-mini",  
    messages=[
        {
            "role": "user",
            "content": [
                { "type": "text",  "text": first_prompt},
                { "type": "image_url", "image_url": { "url": data_url } }
            ]
        }
    ],
    max_tokens=150
)

generated_prompt = response.choices[0].message.content.strip()
print("Generated prompt:", generated_prompt)

image_response = client.images.generate(
    model="dall-e-2",
    prompt=generated_prompt,
    n=1,
    size="1024x1024"
)

image_url = image_response.data[0].url

output_path = "../Resources/ProcessedDrawings/output_image.png"
img_data = requests.get(image_url).content

with open(output_path, "wb") as img_file:
    img_file.write(img_data)

print(f"Image saved to: {output_path}")
