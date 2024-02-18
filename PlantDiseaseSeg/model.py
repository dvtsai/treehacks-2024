from torchvision.models.segmentation.deeplabv3 import DeepLabHead
from torchvision import models
import intel_extension_for_pytorch as ipex

# modified DeepLabv3 model with ResNet101 backbone 
def createDeepLabv3(outputchannels=1):
    # outputchannels=1 because only detecting one thing (disease)
    model = models.segmentation.deeplabv3_resnet101(progress=True)
    model.classifier = DeepLabHead(2048, outputchannels)
    return model
