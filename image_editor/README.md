# Applications of Optical Flow

Optical Flow is a powerful tool for determining how values within an image sequence change over time. At a very high level a basic implementation of optical flow:
1. Determines local characterstics of a given frame
2. Determines the local differences between a given image and the next one in the sequence
3. Uses these differences to create a velocity field for each pixel and each channel within that pixel
4. Updates the target image by applying that velocity field to its pixels

One way to improve optical flow calculations is to take an iterative approach in determining the velocity field by determining the velocity, applying it to the targeted image, determining velocity again, and repeating until convergence. This approach was utalized to achieve a few different results. Some of these explorations with optical flow are described below. 

## Sequence Retiming
Perhaps the most practical application of optical flow is retiming a sequence. Say you have a shot that is a second long, but you need to extend this shot to last for a second and a half. Simply changing the frame rate for the given sequence results in a choppy image. This problem can be solved by using optical flow to determine in between frames, which slows down the shot without causing as many visual stutters. 
One limitation to this approach is that it does not behave well with fast moving objects. A basic limitation of optical flow is limited to only local changes within an image. 

Some results from this can be seen in `OpticalFlowVideos/retiming`. There are currently some subtle artifacts in the retimed video, but this could potentially be solved by using more iterations in the velocity calculation. 

## Artistic Applications
Some more artistic applications of optical flow can be found in `OpticalFlowVideos/stylized`. There were a few approaches attempted in generating these sequences. In general, the images were obtained by determining the velocity by either looking at the next n images, or the previous n images. Looking at the next n images tended to result in the character appearing to jerk ahead, while looking at the previous n images tended to result in more of a motion blurred result. Another interesting result was obtained by reversing the direction of the velocity, then using this newly distorted image to recalculate the velocty again. It resulted in the most distorted image, and could potentially result in some interesting shots if cleaned up.
One thing to note with each of these is how the sequence converges and all artifacts dissapear when there are no longer moving objects in the scene. If this tool were to be used to create a stylized look, some type of subtle motion should likely be added to still shots in order to maintain the stylized look. 

## Proof of concept
Finally, there's the proof of concept shots. These can be found in `OpticalFlowVideos/proof_of_concept`. These were tests I used by applying the flow over the entire sequence to the starting image. This showed how increasing the number of iterations allows for much more accurate predictions of the velocity between frames, allowing a single image to be driven to mimic the entire video. 

