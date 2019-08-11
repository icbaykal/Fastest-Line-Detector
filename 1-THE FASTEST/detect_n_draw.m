function alllines1(param)

global imgEdge;

H_slc=param; 
tic
for(x=1:1000)
[S_OUT_CNT, S_OUT_SX, S_OUT_SY, S_OUT_EX, S_OUT_EY]=stickman_extend(imgEdge,H_slc);
end
t=toc;
'Elapsed time for 1000 iterations in seconds:'
t
'or msecs for 1 iteration.'

number_of_lines=double(S_OUT_CNT);
number_of_lines
figure

sz1=size(imgEdge);
img_w=ones(sz1(1),sz1(2));

imshow(img_w);	
hold on;

for i = 1:S_OUT_CNT
	plot([S_OUT_SX(i) S_OUT_EX(i)], [S_OUT_SY(i) S_OUT_EY(i)],'Color','black');
	%plot(S_OUT_SX(i) , S_OUT_SY(i),'X', 'MarkerSize',6, 'Color','green');
	%plot(S_OUT_EX(i), S_OUT_EY(i),'X', 'MarkerSize',6, 'Color','red');
end


%  figure
% imshow(imgEdge);	
% hold on;
% 
% for i = 1:S_OUT_CNT
% 	plot([S_OUT_SX(i) S_OUT_EX(i)], [S_OUT_SY(i) S_OUT_EY(i)],'LineWidth',2,'Color','blue');
% 	plot(S_OUT_SX(i) , S_OUT_SY(i),'X', 'MarkerSize',6, 'Color','green');
% 	plot(S_OUT_EX(i), S_OUT_EY(i),'X', 'MarkerSize',6, 'Color','red');
% end      
% 
% end
