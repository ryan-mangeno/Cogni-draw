import requests
import os
import time
from dotenv import load_dotenv

load_dotenv()

api_key = os.getenv("MESHY_KEY")

headers = {
    "Authorization": f"Bearer {api_key}"
}

image_path = "../Resources/ProcessedDrawings/output_image.png"
if not os.path.exists(image_path):
    raise FileNotFoundError(f"Image file not found: {image_path}")

with open(image_path, 'rb') as image_file:
    response = requests.post(
        "https://api.meshy.ai/openapi/v1/image-to-3d",
        headers=headers,
        files={"image": image_file},
        data={
            "mode": "preview",
            "art_style": "realistic",
            "should_remesh": "true",
            "negative_prompt": "low quality, low resolution, low poly"
        }
    )
    response.raise_for_status()
    task_id = response.json().get("result")
    if not task_id:
        raise ValueError("No task ID returned from API.")

print("Preview task created. Task ID:", task_id)


status_url = f"https://api.meshy.ai/openapi/v1/image-to-3d/{task_id}"
while True:
    time.sleep(20)
    poll_response = requests.get(status_url, headers=headers)
    poll_response.raise_for_status()
    poll_data = poll_response.json()

    status = poll_data.get("status")
    progress = poll_data.get("progress", 0)

    if status == "SUCCEEDED":
        print("Preview task finished successfully.")
        break
    elif status == "FAILED":
        raise RuntimeError("3D model generation failed.")
    else:
        print(f"Preview task status: {status} | Progress: {progress}%")

model_url = poll_data.get("model_urls", {}).get("obj")
if not model_url:
    raise ValueError("Model URL not found in response")

model_response = requests.get(model_url)
model_response.raise_for_status()

output_path = "generated_model.obj"
with open(output_path, "wb") as f:
    f.write(model_response.content)

print(f"3D model saved as {output_path}")