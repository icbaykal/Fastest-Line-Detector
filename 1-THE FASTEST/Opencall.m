function Opencall(target_frame)

global imgRGB;
global imgGray;
global imgEdge;
global FileName;
global sz1;

[FileName,PathName] = uigetfile('*.jpg;*.tif;*.png;*.gif;*.bmp');
if ~isequal(FileName, 0)
    totalpath=strcat(PathName,FileName);
    img=imread(totalpath);
   
    sz1=size(img);
    imgRGB=uint8(zeros(sz1(1),sz1(2),3));
    sz1=size(sz1);
    if(islogical(img))
        imgGray = uint8(img)*255;
        imgEdge = img;
        imgRGB(:,:,1)=imgGray;imgRGB(:,:,2)=imgGray;imgRGB(:,:,3)=imgGray;    
    elseif(sz1(2)==2)
        imgGray=img;
        imgRGB(:,:,1)=img;imgRGB(:,:,2)=img;imgRGB(:,:,3)=img;        
    elseif(sz1(2)==3)
        imgGray = .2989*img(:,:,1)+.5870*img(:,:,2)+.1140*img(:,:,3);
        imgRGB=img;   
    end
 
    axes(target_frame);
    imshow(imgRGB);	
    



%canny edge detector

%LSD & EDLINES
imgEdge=edge(imgGray,'canny', [0.05 0.18], 1.4);

%treecrop
%imgEdge=edge(imgGray,'canny', [0.15 0.25], 1.5);

%imgEdge=edge(imgGray,'canny', [0.03 0.1], 2);
%applying morpholgical thinning
imgEdge=bwmorph(imgEdge,'thin',Inf);

%to write the image into a file:
t=uint8(imgEdge*255);
imwrite(t,'img1.bmp');

figure
 imshow(imgEdge);

end
