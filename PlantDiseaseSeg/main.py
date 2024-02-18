# code is built off of https://github.com/msminhas93/DeepLabv3FineTuning.git

from pathlib import Path

import torch
from sklearn.metrics import f1_score, roc_auc_score
from torch.utils import data

import datahandler
from model import createDeepLabv3
from trainer import train_model
import intel_extension_for_pytorch as ipex


import argparse
import logging

# Set the logging level for sklearnex to warning or higher to suppress info messages
# surpress the warnings about intel scikit learn being accelerated on the cpu! 
logging.getLogger('sklearnex').setLevel(logging.WARNING)


ap = argparse.ArgumentParser()
ap.add_argument("-d", "--data", required=True,
	help="path to the data directory")
ap.add_argument("-e", "--experiment", required=True,
	help="path to the experiment directory")
ap.add_argument("-p", "--epochs", type=int, required=True,
	help="specify the number of epochs")
ap.add_argument("-b", "--batch", type=int, default=4, required=True,
	help="specify the batch size")
args = vars(ap.parse_args())

data_directory=args["data"]
exp_directory = args["experiment"]
epochs = args["epochs"]
batch_size = args["batch"]


def main(data_directory, exp_directory, epochs, batch_size):
    # Create the deeplabv3 resnet101 model which is pretrained on a subset
    # of COCO train2017, on the 20 categories that are present in the Pascal VOC dataset.
    model = createDeepLabv3()
    model.train()
    data_directory = Path(data_directory)
    # Create the experiment directory if not present
    exp_directory = Path(exp_directory)
    if not exp_directory.exists():
        exp_directory.mkdir()

    # Specify the loss function
    criterion = torch.nn.MSELoss(reduction='mean')
    # Specify the optimizer with a lower learning rate
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-4)

    # Specify the evaluation metrics
    metrics = {'f1_score': f1_score, 'auroc': roc_auc_score}

    # Create the dataloader
    dataloaders = datahandler.get_dataloader_single_folder(
        data_directory, batch_size=batch_size)
    _ = train_model(model,
                    criterion,
                    dataloaders,
                    optimizer,
                    bpath=exp_directory,
                    metrics=metrics,
                    num_epochs=epochs)

    # Save the trained model
    torch.save(model, exp_directory / 'weights.pt')

if __name__ == "__main__":
    main(data_directory, exp_directory, epochs, batch_size)
