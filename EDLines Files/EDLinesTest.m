clear all;
[FileName,PathName] = uigetfile('*.jpg;*.tif;*.png;*.gif;*.bmp');

    totalpath=strcat(PathName,FileName);
    I=imread(totalpath);


tic
for(x=1:200)
    lineSegments = EDLines(I, 1);
end
'For 200 iterations elapsed time in seconds:'
elapsed=toc
'Per iteration elapsed time in msec:'
elapsed/0.2

noLines = size(lineSegments, 1);



dwb_sz=size(I);
dwb_p=ones(dwb_sz(1),dwb_sz(2));
figure;
imshow(dwb_p);   
hold on;


for i = 1:noLines
	plot([lineSegments(i).sx lineSegments(i).ex], [lineSegments(i).sy lineSegments(i).ey],'LineWidth',1,'Color','black');
end
hold off;
% figure
% dwb_p1=zeros(dwb_sz(1),dwb_sz(2));
% imshow(dwb_p1);   
% hold on;
% for i = 1:noLines
% 	plot([lineSegments(i).sx lineSegments(i).ex], [lineSegments(i).sy lineSegments(i).ey],'LineWidth',2,'Color','blue');
% 	plot(lineSegments(i).sx, lineSegments(i).sy,'X', 'MarkerSize',6, 'Color','green');
% 	plot(lineSegments(i).ex,lineSegments(i).ey, 'X', 'MarkerSize',6, 'Color','red');
% end

