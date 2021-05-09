# SP21-IN-CSCI-55800-26854----Edge-detection

For the image used in last assignment,

(1) Computing a gradient image (all positive values) from the horizontal and vertical differential images (containing both positive and negative values so far). Display both results for 3*3 and 5*5 filters.

(2) Detecting edges at the  peaks of gradient values both horizontally and vertically over a threshold. The threshold can be selected manually through multiple trial and error so that the edge results show a good shape of scenes. Edge must be one pixel wide and marked in a bright intensity. Places without edge are set to zero value in the output.

Take care of overflow and underflow, if any, by normalizing filtered results into the range of a byte.

