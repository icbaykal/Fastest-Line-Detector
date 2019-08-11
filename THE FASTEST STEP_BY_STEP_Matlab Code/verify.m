function verify(p_ax)

global imgEdge;
global sz2;
global ds_e_matrx;


seg_plt=zeros(sz2(1),sz2(2));

axes(p_ax);
imshow(imgEdge); hold on;

sz4=size(ds_e_matrx);
 for i=1:sz4(3)
	if ds_e_matrx(:,:,i)>0
		plot(ds_e_matrx(1,:,i),ds_e_matrx(2,:,i),'LineWidth',2,'Color','blue')
		plot(ds_e_matrx(1,1,i),ds_e_matrx(2,1,i),'X', 'MarkerSize',6, 'Color','green')
		plot(ds_e_matrx(1,2,i),ds_e_matrx(2,2,i),'X', 'MarkerSize',6, 'Color','red')
	end
end


