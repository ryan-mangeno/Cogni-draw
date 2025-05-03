from openai_api import generate_image_from_drawing
from meshy_api import gen_model_from_image 

import time

if __name__ == "__main__":
    # gen image from drawing
    #generate_image_from_drawing()

    time.sleep(2) # testing modelbuffer, remove later

    # gen 3D model from the generated image
    #gen_model_from_image()

    # paths are hardcoded so we dont pass as args to the functions