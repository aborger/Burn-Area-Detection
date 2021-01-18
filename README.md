# Burn-Area-Detection

This program is part of NNU's Fire Monitoring and Assessment Platform (FireMAP). It identifies and counting tree stands (forested pockets) in a grassland. It also detects burn areas under tree stands, and identifies unburnt tree stands and grassland islands. Shadows mislabeled as burn areas are also detected.

The program uses ascii files as inputs. With the following character representations:

- g = Grassland
- t = Forested Area
- b = Burn Area
- B = Burn Area Under Forested Area

The tree stand count is accomplished by recursively traversing the file cell by cell and labeling each tree stand with a unique identifier and the number of cells it covers. Burn areas under tree stands are identified by a tree stand completely surrounded by burn pixels. If a tree stand contains grassland pixels it is identified as an unburnt island. Burn areas with less than 4 pixels are identified as shadows and corrected to be grassland pixels.

Test files are included for both the tree stands and burn detection systems. One test file contains a section of the Bitterroot national forest which was captured by a drone. This file is rather large and will need to be outputted to a file (You will be prompted when necessary).
