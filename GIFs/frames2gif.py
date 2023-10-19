import re
from PIL import Image
import os
import contextlib

def get_number(file_name):
    m = re.findall('frame_([0-9]*)\.', file_name)[0]
    return int(m)

def createGIF(outputPath, framesPath):
    numberedFrames = [[get_number(fName), os.path.join(framesPath, fName)] for fName in os.listdir(framesPath)]

    if len(numberedFrames) == 0 :
        return
    
    with contextlib.ExitStack() as stack:
        frames = (stack.enter_context(Image.open(f[1])) for f in sorted(numberedFrames))

        
        
        frames = (f.resize((800, 800), Image.Resampling.NEAREST) for f in frames)

        firstFrame = next(frames)

        firstFrame.save(fp=outputPath, format='GIF', append_images=frames, save_all=True, duration=1, loop=0)


directory = os.path.dirname(__file__)
bfsFramesPath = directory + "/bfsFrames"
aStarFramesPath = directory + "/aStarFrames"
dfsFramesPath = directory + "/dfsFrames"
bestFirstFramesPath = directory + "/bestFirstFrames"

createGIF(directory + "/bfsAnimation.gif", bfsFramesPath)
createGIF(directory + "/aStarAnimation.gif", aStarFramesPath)
createGIF(directory + "/dfsAnimation.gif", dfsFramesPath)
createGIF(directory + "/bestFirstAnimation.gif", bestFirstFramesPath)