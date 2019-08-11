function edgedet(p_ax)

global imgEdge;
global imgGray;
global sz2;
global sz3;

imgEdge=edge(imgGray,'canny', [0.1 0.15], 1);


imgEdge=bwmorph(imgEdge,'thin');

sz_imgEdge=size(imgEdge);
z_imgEdge=zeros(sz_imgEdge(1)+8,sz_imgEdge(2)+8);
z_imgEdge=logical(z_imgEdge);
z_imgEdge(5:sz_imgEdge(1)+4,5:sz_imgEdge(2)+4)=imgEdge;

imgEdge=z_imgEdge;
sz2=size(imgEdge);

y_k=1;
for(y=1:4:sz2(2)-4)
	y_k=y_k+1;
end
sz3(2)=y_k;

x_k=1;
for(y=1:4:sz2(1)-4)
	x_k=x_k+1;
end
sz3(1)=x_k;


imgScr=uint8(imgEdge)*255;
axes(p_ax);
imshow(imgScr);
