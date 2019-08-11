function piecewisematch(p_ax)

global imgEdge;
global seg;
global dlinematrx;
global sz2;
global sz3;



load('file_singles.mat')

[seg, img2, dlinematrx]  = piecewisematch_mex_mex(sz3, sz2, imgEdge, filesingles);

axes(p_ax);
img2=uint8(img2);
imshow(img2);

		