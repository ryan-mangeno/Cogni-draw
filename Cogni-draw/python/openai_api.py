from openai import OpenAI
from dotenv import load_dotenv
import os
import requests


load_dotenv()
api_key = os.getenv("API_KEY")
client = OpenAI(api_key=api_key)


image_path = "Resources/Drawings/drawing.png"
prompt = "Transform this rough 2D sketch into a detailed 3D version with realistic depth and volume.\
          Keep the proportions, structure, and outline from the original sketch while enhancing its \
          three-dimensional appearance with proper lighting, shading, and depth. \
          The background should remain solid black."


response = client.images.edit(
    model="dall-e-2",
    image=open(image_path, "rb"),
    prompt=prompt,
    n=1,
    size="1024x1024",
)

image_url = response.data[0].url
img_data = requests.get(image_url).content
output_path = "../Resources/ProcessedDrawings/output_image.png"

with open(output_path, "wb") as img_file:
    img_file.write(img_data)


