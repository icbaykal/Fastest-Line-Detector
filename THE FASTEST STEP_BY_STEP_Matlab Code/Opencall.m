
function Opencall(target_frame)


global imgGray;




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
    
  
end
