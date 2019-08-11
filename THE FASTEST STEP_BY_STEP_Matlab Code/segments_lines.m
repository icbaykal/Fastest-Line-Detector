function segments_lines(p_ax)


global seg;
global imgEdge;
global sz2;
global sz3;
global dlinematrx;
global ds_e_matrx;



seg  = segments_lines_mex_mex(seg, sz3, dlinematrx);


   
seg(:,:,17)=seg(:,:,16);
df_linm1=1;
   for i=2:sz3(1)-1
      for j=2:sz3(2)-1
              if seg(i,j,17)>0
                 len_seg1(1,df_linm1)=seg(i,j,17);
                 df_linm1=df_linm1+1;
				 
              end
       end 
   end

[segcnt_1,segnum_1]=hist(len_seg1(1,:),unique(len_seg1(1,:)));
f_linm1=size(segcnt_1);
f_lin_m=1;
for i=1:f_linm1(2)
	y_r1=rem(segnum_1(i),sz3(2));
	x_r1=((segnum_1(i)-y_r1)/sz3(2))+1;
	seg(x_r1,y_r1,18)=segcnt_1(i);
end	

seg_min_len1=2;
for i=2:sz3(1)-1
    for j=2:sz3(2)-1
		if seg(i,j,17)>0
			y_r2=rem(seg(i,j,17),sz3(2));
			x_r2=((seg(i,j,17)-y_r2)/sz3(2))+1;
			if seg(x_r2,y_r2,18)<seg_min_len1
				seg(i,j,17)=0;
			end
		end
	end
end

	

 seg(:,:,25)=seg(:,:,21);
 seg(:,:,26)=seg(:,:,23);	
ds_e_matrx=zeros(2,2,sz3(2)*sz3(1));
	for j=2:sz3(2)-1
		for i=2:sz3(1)-1
      
              if seg(i,j,17)>0
                 if (seg(i,j,25)~=0)& (ds_e_matrx(1,1,seg(i,j,17))==0) 
					ds_e_matrx(1,1,seg(i,j,17))=seg(i,j,21);
					ds_e_matrx(2,1,seg(i,j,17))=seg(i,j,22);
					seg(i,j,25)=0;
				 end
				 if (seg(i,j,25)==0)& (seg(i,j,26)~=0) & (ds_e_matrx(1,1,seg(i,j,17))==0)
					ds_e_matrx(1,1,seg(i,j,17))=seg(i,j,23);
					ds_e_matrx(2,1,seg(i,j,17))=seg(i,j,24);
					seg(i,j,26)=0;
				 end
				 if (seg(i,j,25)~=0)& (ds_e_matrx(1,1,seg(i,j,17))~=0) 
					ds_e_matrx(1,2,seg(i,j,17))=seg(i,j,21);
					ds_e_matrx(2,2,seg(i,j,17))=seg(i,j,22);
					seg(i,j,25)=0;
				 end
				 if (seg(i,j,25)==0)& (seg(i,j,26)~=0)& (ds_e_matrx(1,1,seg(i,j,17))~=0) 
					ds_e_matrx(1,2,seg(i,j,17))=seg(i,j,23);
					ds_e_matrx(2,2,seg(i,j,17))=seg(i,j,24);
					seg(i,j,26)=0;
				 end
				 
              end
       end 
   end



imgSlope2=uint8(zeros(sz2(1),sz2(2),3));
imgSlope2(:,:,1)=uint8(imgEdge)*255;
imgSlope2(:,:,2)=uint8(imgEdge)*255;
imgSlope2(:,:,3)=uint8(imgEdge)*255;
for x=1:sz3
    for y=1:sz3(2)
        rgbx=(x-1)*4+1;
        rgby=(y-1)*4+1;
        if (mod(seg(x,y,17),9)==1)imgSlope2(rgbx:rgbx+3,rgby:rgby+3,1)=255;
        end                               
        if (mod(seg(x,y,17),9)==2)imgSlope2(rgbx:rgbx+3,rgby:rgby+3,2)=255;            
        end                               
        if (mod(seg(x,y,17),9)==3)imgSlope2(rgbx:rgbx+3,rgby:rgby+3,3)=255;
        end         
        if (mod(seg(x,y,17),9)==4)
				if imgSlope2(rgbx,rgby,1)==0 	imgSlope2(rgbx,rgby,1)=240;
                end
                if imgSlope2(rgbx+1,rgby,1)==0 imgSlope2(rgbx+1,rgby,1)=240;
                end
                if imgSlope2(rgbx+2,rgby,1)==0 imgSlope2(rgbx+2,rgby,1)=240;
                end
                if imgSlope2(rgbx+3,rgby,1)==0 imgSlope2(rgbx+3,rgby,1)=240;
                end
           
				if imgSlope2(rgbx,rgby+1,1)==0 	imgSlope2(rgbx,rgby+1,1)=240;
                end
                if imgSlope2(rgbx+1,rgby+1,1)==0 imgSlope2(rgbx+1,rgby+1,1)=240;
                end
                if imgSlope2(rgbx+2,rgby+1,1)==0 imgSlope2(rgbx+2,rgby+1,1)=240;
                end
                if imgSlope2(rgbx+3,rgby+1,1)==0 imgSlope2(rgbx+3,rgby+1,1)=240;
                end
             
				if imgSlope2(rgbx,rgby+2,1)==0 	imgSlope2(rgbx,rgby+2,1)=240;
                end
                if imgSlope2(rgbx+1,rgby+2,1)==0 imgSlope2(rgbx+1,rgby+2,1)=240;
                end
                if imgSlope2(rgbx+2,rgby+2,1)==0 imgSlope2(rgbx+2,rgby+2,1)=240;
                end
                if imgSlope2(rgbx+3,rgby+2,1)==0 imgSlope2(rgbx+3,rgby+2,1)=240;
                end
            
				if imgSlope2(rgbx,rgby+3,1)==0 	imgSlope2(rgbx,rgby+3,1)=240;
                end
                if imgSlope2(rgbx+1,rgby+3,1)==0 imgSlope2(rgbx+1,rgby+3,1)=240;
                end
                if imgSlope2(rgbx+2,rgby+3,1)==0 imgSlope2(rgbx+2,rgby+3,1)=240;
                end
                if imgSlope2(rgbx+3,rgby+3,1)==0 imgSlope2(rgbx+3,rgby+3,1)=240;
                end
                
				if imgSlope2(rgbx,rgby,2)==0 	imgSlope2(rgbx,rgby,2)=251;
                end
                if imgSlope2(rgbx+1,rgby,2)==0 imgSlope2(rgbx+1,rgby,2)=251;
                end
                if imgSlope2(rgbx+2,rgby,2)==0 imgSlope2(rgbx+2,rgby,2)=251;
                end
                if imgSlope2(rgbx+3,rgby,2)==0 imgSlope2(rgbx+3,rgby,2)=251;
                end
            
				if imgSlope2(rgbx,rgby+1,2)==0 	imgSlope2(rgbx,rgby+1,2)=251;
                end
                if imgSlope2(rgbx+1,rgby+1,2)==0 imgSlope2(rgbx+1,rgby+1,2)=251;
                end
                if imgSlope2(rgbx+2,rgby+1,2)==0 imgSlope2(rgbx+2,rgby+1,2)=251;
                end
                if imgSlope2(rgbx+3,rgby+1,2)==0 imgSlope2(rgbx+3,rgby+1,2)=251;
                end
              
				if imgSlope2(rgbx,rgby+2,2)==0 	imgSlope2(rgbx,rgby+2,2)=251;
                end
                if imgSlope2(rgbx+1,rgby+2,2)==0 imgSlope2(rgbx+1,rgby+2,2)=251;
                end
                if imgSlope2(rgbx+2,rgby+2,2)==0 imgSlope2(rgbx+2,rgby+2,2)=251;
                end
                if imgSlope2(rgbx+3,rgby+2,2)==0 imgSlope2(rgbx+3,rgby+2,2)=251;
                end
            
				if imgSlope2(rgbx,rgby+3,2)==0 	imgSlope2(rgbx,rgby+3,2)=251;
                end
                if imgSlope2(rgbx+1,rgby+3,2)==0 imgSlope2(rgbx+1,rgby+3,2)=251;
                end
                if imgSlope2(rgbx+2,rgby+3,2)==0 imgSlope2(rgbx+2,rgby+3,2)=251;
                end
                if imgSlope2(rgbx+3,rgby+3,2)==0 imgSlope2(rgbx+3,rgby+3,2)=251;
                end
        end         
        if (mod(seg(x,y,17),9)==5)
				if imgSlope2(rgbx,rgby,1)==0 	imgSlope2(rgbx,rgby,1)=240;
                end
                if imgSlope2(rgbx+1,rgby,1)==0 imgSlope2(rgbx+1,rgby,1)=240;
                end
                if imgSlope2(rgbx+2,rgby,1)==0 imgSlope2(rgbx+2,rgby,1)=240;
                end
                if imgSlope2(rgbx+3,rgby,1)==0 imgSlope2(rgbx+3,rgby,1)=240;
                end
            
				if imgSlope2(rgbx,rgby+1,1)==0 	imgSlope2(rgbx,rgby+1,1)=240;
                end
                if imgSlope2(rgbx+1,rgby+1,1)==0 imgSlope2(rgbx+1,rgby+1,1)=240;
                end
                if imgSlope2(rgbx+2,rgby+1,1)==0 imgSlope2(rgbx+2,rgby+1,1)=240;
                end
                if imgSlope2(rgbx+3,rgby+1,1)==0 imgSlope2(rgbx+3,rgby+1,1)=240;
                end
              
				if imgSlope2(rgbx,rgby+2,1)==0 	imgSlope2(rgbx,rgby+2,1)=240;
                end
                if imgSlope2(rgbx+1,rgby+2,1)==0 imgSlope2(rgbx+1,rgby+2,1)=240;
                end
                if imgSlope2(rgbx+2,rgby+2,1)==0 imgSlope2(rgbx+2,rgby+2,1)=240;
                end
                if imgSlope2(rgbx+3,rgby+2,1)==0 imgSlope2(rgbx+3,rgby+2,1)=240;
                end
            
				if imgSlope2(rgbx,rgby+3,1)==0 	imgSlope2(rgbx,rgby+3,1)=240;
                end
                if imgSlope2(rgbx+1,rgby+3,1)==0 imgSlope2(rgbx+1,rgby+3,1)=240;
                end
                if imgSlope2(rgbx+2,rgby+3,1)==0 imgSlope2(rgbx+2,rgby+3,1)=240;
                end
                if imgSlope2(rgbx+3,rgby+3,1)==0 imgSlope2(rgbx+3,rgby+3,1)=240;
                end
                
				if imgSlope2(rgbx,rgby,3)==0 	imgSlope2(rgbx,rgby,3)=252;
                end
                if imgSlope2(rgbx+1,rgby,3)==0 imgSlope2(rgbx+1,rgby,3)=252;
                end
                if imgSlope2(rgbx+2,rgby,3)==0 imgSlope2(rgbx+2,rgby,3)=252;
                end
                if imgSlope2(rgbx+3,rgby,3)==0 imgSlope2(rgbx+3,rgby,3)=252;
                end
            
				if imgSlope2(rgbx,rgby+1,3)==0 	imgSlope2(rgbx,rgby+1,3)=252;
                end
                if imgSlope2(rgbx+1,rgby+1,3)==0 imgSlope2(rgbx+1,rgby+1,3)=252;
                end
                if imgSlope2(rgbx+2,rgby+1,3)==0 imgSlope2(rgbx+2,rgby+1,3)=252;
                end
                if imgSlope2(rgbx+3,rgby+1,3)==0 imgSlope2(rgbx+3,rgby+1,3)=252;
                end
              
				if imgSlope2(rgbx,rgby+2,3)==0 	imgSlope2(rgbx,rgby+2,3)=252;
                end
                if imgSlope2(rgbx+1,rgby+2,3)==0 imgSlope2(rgbx+1,rgby+2,3)=252;
                end
                if imgSlope2(rgbx+2,rgby+2,3)==0 imgSlope2(rgbx+2,rgby+2,3)=252;
                end
                if imgSlope2(rgbx+3,rgby+2,3)==0 imgSlope2(rgbx+3,rgby+2,3)=252;
                end
            
				if imgSlope2(rgbx,rgby+3,3)==0 	imgSlope2(rgbx,rgby+3,3)=252;
                end
                if imgSlope2(rgbx+1,rgby+3,3)==0 imgSlope2(rgbx+1,rgby+3,3)=252;
                end
                if imgSlope2(rgbx+2,rgby+3,3)==0 imgSlope2(rgbx+2,rgby+3,3)=252;
                end
                if imgSlope2(rgbx+3,rgby+3,3)==0 imgSlope2(rgbx+3,rgby+3,3)=252;
                end
        end         
        if (mod(seg(x,y,17),9)==6)
				if imgSlope2(rgbx,rgby,2)==0 	imgSlope2(rgbx,rgby,2)=240;
                end
                if imgSlope2(rgbx+1,rgby,2)==0 imgSlope2(rgbx+1,rgby,2)=240;
                end
                if imgSlope2(rgbx+2,rgby,2)==0 imgSlope2(rgbx+2,rgby,2)=240;
                end
                if imgSlope2(rgbx+3,rgby,2)==0 imgSlope2(rgbx+3,rgby,2)=240;
                end
            
				if imgSlope2(rgbx,rgby+1,2)==0 	imgSlope2(rgbx,rgby+1,2)=240;
                end
                if imgSlope2(rgbx+1,rgby+1,2)==0 imgSlope2(rgbx+1,rgby+1,2)=240;
                end
                if imgSlope2(rgbx+2,rgby+1,2)==0 imgSlope2(rgbx+2,rgby+1,2)=240;
                end
                if imgSlope2(rgbx+3,rgby+1,2)==0 imgSlope2(rgbx+3,rgby+1,2)=240;
                end
              
				if imgSlope2(rgbx,rgby+2,2)==0 	imgSlope2(rgbx,rgby+2,2)=240;
                end
                if imgSlope2(rgbx+1,rgby+2,2)==0 imgSlope2(rgbx+1,rgby+2,2)=240;
                end
                if imgSlope2(rgbx+2,rgby+2,2)==0 imgSlope2(rgbx+2,rgby+2,2)=240;
                end
                if imgSlope2(rgbx+3,rgby+2,2)==0 imgSlope2(rgbx+3,rgby+2,2)=240;
                end
            
				if imgSlope2(rgbx,rgby+3,2)==0 	imgSlope2(rgbx,rgby+3,2)=240;
                end
                if imgSlope2(rgbx+1,rgby+3,2)==0 imgSlope2(rgbx+1,rgby+3,2)=240;
                end
                if imgSlope2(rgbx+2,rgby+3,2)==0 imgSlope2(rgbx+2,rgby+3,2)=240;
                end
                if imgSlope2(rgbx+3,rgby+3,2)==0 imgSlope2(rgbx+3,rgby+3,2)=240;
                end
                
				if imgSlope2(rgbx,rgby,3)==0 	imgSlope2(rgbx,rgby,3)=240;
                end
                if imgSlope2(rgbx+1,rgby,3)==0 imgSlope2(rgbx+1,rgby,3)=240;
                end
                if imgSlope2(rgbx+2,rgby,3)==0 imgSlope2(rgbx+2,rgby,3)=240;
                end
                if imgSlope2(rgbx+3,rgby,3)==0 imgSlope2(rgbx+3,rgby,3)=240;
                end
           
				if imgSlope2(rgbx,rgby+1,3)==0 	imgSlope2(rgbx,rgby+1,3)=240;
                end
                if imgSlope2(rgbx+1,rgby+1,3)==0 imgSlope2(rgbx+1,rgby+1,3)=240;
                end
                if imgSlope2(rgbx+2,rgby+1,3)==0 imgSlope2(rgbx+2,rgby+1,3)=240;
                end
                if imgSlope2(rgbx+3,rgby+1,3)==0 imgSlope2(rgbx+3,rgby+1,3)=240;
                end
             
				if imgSlope2(rgbx,rgby+2,3)==0 	imgSlope2(rgbx,rgby+2,3)=240;
                end
                if imgSlope2(rgbx+1,rgby+2,3)==0 imgSlope2(rgbx+1,rgby+2,3)=240;
                end
                if imgSlope2(rgbx+2,rgby+2,3)==0 imgSlope2(rgbx+2,rgby+2,3)=240;
                end
                if imgSlope2(rgbx+3,rgby+2,3)==0 imgSlope2(rgbx+3,rgby+2,3)=240;
                end
            
				if imgSlope2(rgbx,rgby+3,3)==0 	imgSlope2(rgbx,rgby+3,3)=240;
                end
                if imgSlope2(rgbx+1,rgby+3,3)==0 imgSlope2(rgbx+1,rgby+3,3)=240;
                end
                if imgSlope2(rgbx+2,rgby+3,3)==0 imgSlope2(rgbx+2,rgby+3,3)=240;
                end
                if imgSlope2(rgbx+3,rgby+3,3)==0 imgSlope2(rgbx+3,rgby+3,3)=240;
                end
		end        
        if (mod(seg(x,y,17),9)==7)
				if imgSlope2(rgbx,rgby,1)==0 	imgSlope2(rgbx,rgby,1)=140;
                end
                if imgSlope2(rgbx+1,rgby,1)==0 imgSlope2(rgbx+1,rgby,1)=140;
                end
                if imgSlope2(rgbx+2,rgby,1)==0 imgSlope2(rgbx+2,rgby,1)=140;
                end
                if imgSlope2(rgbx+3,rgby,1)==0 imgSlope2(rgbx+3,rgby,1)=140;
                end
            
				if imgSlope2(rgbx,rgby+1,1)==0 	imgSlope2(rgbx,rgby+1,1)=140;
                end
                if imgSlope2(rgbx+1,rgby+1,1)==0 imgSlope2(rgbx+1,rgby+1,1)=140;
                end
                if imgSlope2(rgbx+2,rgby+1,1)==0 imgSlope2(rgbx+2,rgby+1,1)=140;
                end
                if imgSlope2(rgbx+3,rgby+1,1)==0 imgSlope2(rgbx+3,rgby+1,1)=140;
                end
              
				if imgSlope2(rgbx,rgby+2,1)==0 	imgSlope2(rgbx,rgby+2,1)=140;
                end
                if imgSlope2(rgbx+1,rgby+2,1)==0 imgSlope2(rgbx+1,rgby+2,1)=140;
                end
                if imgSlope2(rgbx+2,rgby+2,1)==0 imgSlope2(rgbx+2,rgby+2,1)=140;
                end
                if imgSlope2(rgbx+3,rgby+2,1)==0 imgSlope2(rgbx+3,rgby+2,1)=140;
                end
            
				if imgSlope2(rgbx,rgby+3,1)==0 	imgSlope2(rgbx,rgby+3,1)=140;
                end
                if imgSlope2(rgbx+1,rgby+3,1)==0 imgSlope2(rgbx+1,rgby+3,1)=140;
                end
                if imgSlope2(rgbx+2,rgby+3,1)==0 imgSlope2(rgbx+2,rgby+3,1)=140;
                end
                if imgSlope2(rgbx+3,rgby+3,1)==0 imgSlope2(rgbx+3,rgby+3,1)=140;
                end
                
				if imgSlope2(rgbx,rgby,2)==0 	imgSlope2(rgbx,rgby,2)=80;
                end
                if imgSlope2(rgbx+1,rgby,2)==0 imgSlope2(rgbx+1,rgby,2)=80;
                end
                if imgSlope2(rgbx+2,rgby,2)==0 imgSlope2(rgbx+2,rgby,2)=80;
                end
                if imgSlope2(rgbx+3,rgby,2)==0 imgSlope2(rgbx+3,rgby,2)=80;
                end
            
				if imgSlope2(rgbx,rgby+1,2)==0 	imgSlope2(rgbx,rgby+1,2)=80;
                end
                if imgSlope2(rgbx+1,rgby+1,2)==0 imgSlope2(rgbx+1,rgby+1,2)=80;
                end
                if imgSlope2(rgbx+2,rgby+1,2)==0 imgSlope2(rgbx+2,rgby+1,2)=80;
                end
                if imgSlope2(rgbx+3,rgby+1,2)==0 imgSlope2(rgbx+3,rgby+1,2)=80;
                end
              
				if imgSlope2(rgbx,rgby+2,2)==0 	imgSlope2(rgbx,rgby+2,2)=80;
                end
                if imgSlope2(rgbx+1,rgby+2,2)==0 imgSlope2(rgbx+1,rgby+2,2)=80;
                end
                if imgSlope2(rgbx+2,rgby+2,2)==0 imgSlope2(rgbx+2,rgby+2,2)=80;
                end
                if imgSlope2(rgbx+3,rgby+2,2)==0 imgSlope2(rgbx+3,rgby+2,2)=80;
                end
            
				if imgSlope2(rgbx,rgby+3,2)==0 	imgSlope2(rgbx,rgby+3,2)=80;
                end
                if imgSlope2(rgbx+1,rgby+3,2)==0 imgSlope2(rgbx+1,rgby+3,2)=80;
                end
                if imgSlope2(rgbx+2,rgby+3,2)==0 imgSlope2(rgbx+2,rgby+3,2)=80;
                end
                if imgSlope2(rgbx+3,rgby+3,2)==0 imgSlope2(rgbx+3,rgby+3,2)=80;
                end
                
				if imgSlope2(rgbx,rgby,3)==0 	imgSlope2(rgbx,rgby,3)=40;
                end
                if imgSlope2(rgbx+1,rgby,3)==0 imgSlope2(rgbx+1,rgby,3)=40;
                end
                if imgSlope2(rgbx+2,rgby,3)==0 imgSlope2(rgbx+2,rgby,3)=40;
                end
                if imgSlope2(rgbx+3,rgby,3)==0 imgSlope2(rgbx+3,rgby,3)=40;
                end
            
				if imgSlope2(rgbx,rgby+1,3)==0 	imgSlope2(rgbx,rgby+1,3)=40;
                end
                if imgSlope2(rgbx+1,rgby+1,3)==0 imgSlope2(rgbx+1,rgby+1,3)=40;
                end
                if imgSlope2(rgbx+2,rgby+1,3)==0 imgSlope2(rgbx+2,rgby+1,3)=40;
                end
                if imgSlope2(rgbx+3,rgby+1,3)==0 imgSlope2(rgbx+3,rgby+1,3)=40;
                end
             
				if imgSlope2(rgbx,rgby+2,3)==0 	imgSlope2(rgbx,rgby+2,3)=40;
                end
                if imgSlope2(rgbx+1,rgby+2,3)==0 imgSlope2(rgbx+1,rgby+2,3)=40;
                end
                if imgSlope2(rgbx+2,rgby+2,3)==0 imgSlope2(rgbx+2,rgby+2,3)=40;
                end
                if imgSlope2(rgbx+3,rgby+2,3)==0 imgSlope2(rgbx+3,rgby+2,3)=40;
                end
            
				if imgSlope2(rgbx,rgby+3,3)==0 	imgSlope2(rgbx,rgby+3,3)=40;
                end
                if imgSlope2(rgbx+1,rgby+3,3)==0 imgSlope2(rgbx+1,rgby+3,3)=40;
                end
                if imgSlope2(rgbx+2,rgby+3,3)==0 imgSlope2(rgbx+2,rgby+3,3)=40;
                end
                if imgSlope2(rgbx+3,rgby+3,3)==0 imgSlope2(rgbx+3,rgby+3,3)=40;
                end
		end         
        if (mod(seg(x,y,17),9)==8)
				if imgSlope2(rgbx,rgby,1)==0 	imgSlope2(rgbx,rgby,1)=80;
                end
                if imgSlope2(rgbx+1,rgby,1)==0 imgSlope2(rgbx+1,rgby,1)=80;
                end
                if imgSlope2(rgbx+2,rgby,1)==0 imgSlope2(rgbx+2,rgby,1)=80;
                end
                if imgSlope2(rgbx+3,rgby,1)==0 imgSlope2(rgbx+3,rgby,1)=80;
                end
            
				if imgSlope2(rgbx,rgby+1,1)==0 	imgSlope2(rgbx,rgby+1,1)=80;
                end
                if imgSlope2(rgbx+1,rgby+1,1)==0 imgSlope2(rgbx+1,rgby+1,1)=80;
                end
                if imgSlope2(rgbx+2,rgby+1,1)==0 imgSlope2(rgbx+2,rgby+1,1)=80;
                end
                if imgSlope2(rgbx+3,rgby+1,1)==0 imgSlope2(rgbx+3,rgby+1,1)=80;
                end
              
				if imgSlope2(rgbx,rgby+2,1)==0 	imgSlope2(rgbx,rgby+2,1)=80;
                end
                if imgSlope2(rgbx+1,rgby+2,1)==0 imgSlope2(rgbx+1,rgby+2,1)=80;
                end
                if imgSlope2(rgbx+2,rgby+2,1)==0 imgSlope2(rgbx+2,rgby+2,1)=80;
                end
                if imgSlope2(rgbx+3,rgby+2,1)==0 imgSlope2(rgbx+3,rgby+2,1)=80;
                end
            
				if imgSlope2(rgbx,rgby+3,1)==0 	imgSlope2(rgbx,rgby+3,1)=80;
                end
                if imgSlope2(rgbx+1,rgby+3,1)==0 imgSlope2(rgbx+1,rgby+3,1)=80;
                end
                if imgSlope2(rgbx+2,rgby+3,1)==0 imgSlope2(rgbx+2,rgby+3,1)=80;
                end
                if imgSlope2(rgbx+3,rgby+3,1)==0 imgSlope2(rgbx+3,rgby+3,1)=80;
                end
                
				if imgSlope2(rgbx,rgby,2)==0 	imgSlope2(rgbx,rgby,2)=40;
                end
                if imgSlope2(rgbx+1,rgby,2)==0 imgSlope2(rgbx+1,rgby,2)=40;
                end
                if imgSlope2(rgbx+2,rgby,2)==0 imgSlope2(rgbx+2,rgby,2)=40;
                end
                if imgSlope2(rgbx+3,rgby,2)==0 imgSlope2(rgbx+3,rgby,2)=40;
                end
            
				if imgSlope2(rgbx,rgby+1,2)==0 	imgSlope2(rgbx,rgby+1,2)=40;
                end
                if imgSlope2(rgbx+1,rgby+1,2)==0 imgSlope2(rgbx+1,rgby+1,2)=40;
                end
                if imgSlope2(rgbx+2,rgby+1,2)==0 imgSlope2(rgbx+2,rgby+1,2)=40;
                end
                if imgSlope2(rgbx+3,rgby+1,2)==0 imgSlope2(rgbx+3,rgby+1,2)=40;
                end
              
				if imgSlope2(rgbx,rgby+2,2)==0 	imgSlope2(rgbx,rgby+2,2)=40;
                end
                if imgSlope2(rgbx+1,rgby+2,2)==0 imgSlope2(rgbx+1,rgby+2,2)=40;
                end
                if imgSlope2(rgbx+2,rgby+2,2)==0 imgSlope2(rgbx+2,rgby+2,2)=40;
                end
                if imgSlope2(rgbx+3,rgby+2,2)==0 imgSlope2(rgbx+3,rgby+2,2)=40;
                end
            
				if imgSlope2(rgbx,rgby+3,2)==0 	imgSlope2(rgbx,rgby+3,2)=40;
                end
                if imgSlope2(rgbx+1,rgby+3,2)==0 imgSlope2(rgbx+1,rgby+3,2)=40;
                end
                if imgSlope2(rgbx+2,rgby+3,2)==0 imgSlope2(rgbx+2,rgby+3,2)=40;
                end
                if imgSlope2(rgbx+3,rgby+3,2)==0 imgSlope2(rgbx+3,rgby+3,2)=40;
                end
               
				if imgSlope2(rgbx,rgby,3)==0 	imgSlope2(rgbx,rgby,3)=140;
                end
                if imgSlope2(rgbx+1,rgby,3)==0 imgSlope2(rgbx+1,rgby,3)=140;
                end
                if imgSlope2(rgbx+2,rgby,3)==0 imgSlope2(rgbx+2,rgby,3)=140;
                end
                if imgSlope2(rgbx+3,rgby,3)==0 imgSlope2(rgbx+3,rgby,3)=140;
                end
            
				if imgSlope2(rgbx,rgby+1,3)==0 	imgSlope2(rgbx,rgby+1,3)=140;
                end
                if imgSlope2(rgbx+1,rgby+1,3)==0 imgSlope2(rgbx+1,rgby+1,3)=140;
                end
                if imgSlope2(rgbx+2,rgby+1,3)==0 imgSlope2(rgbx+2,rgby+1,3)=140;
                end
                if imgSlope2(rgbx+3,rgby+1,3)==0 imgSlope2(rgbx+3,rgby+1,3)=140;
                end
              
				if imgSlope2(rgbx,rgby+2,3)==0 	imgSlope2(rgbx,rgby+2,3)=140;
                end
                if imgSlope2(rgbx+1,rgby+2,3)==0 imgSlope2(rgbx+1,rgby+2,3)=140;
                end
                if imgSlope2(rgbx+2,rgby+2,3)==0 imgSlope2(rgbx+2,rgby+2,3)=140;
                end
                if imgSlope2(rgbx+3,rgby+2,3)==0 imgSlope2(rgbx+3,rgby+2,3)=140;
                end
            
				if imgSlope2(rgbx,rgby+3,3)==0 	imgSlope2(rgbx,rgby+3,3)=140;
                end
                if imgSlope2(rgbx+1,rgby+3,3)==0 imgSlope2(rgbx+1,rgby+3,3)=140;
                end
                if imgSlope2(rgbx+2,rgby+3,3)==0 imgSlope2(rgbx+2,rgby+3,3)=140;
                end
                if imgSlope2(rgbx+3,rgby+3,3)==0 imgSlope2(rgbx+3,rgby+3,3)=140;
                end
		end
		if(mod(seg(x,y,17),9)==0 & seg(x,y,17)~=0 ) 
			if imgSlope2(rgbx,rgby,1)==0 	imgSlope2(rgbx,rgby,1)=80;
                end
                if imgSlope2(rgbx+1,rgby,1)==0 imgSlope2(rgbx+1,rgby,1)=80;
                end
                if imgSlope2(rgbx+2,rgby,1)==0 imgSlope2(rgbx+2,rgby,1)=80;
                end
                if imgSlope2(rgbx+3,rgby,1)==0 imgSlope2(rgbx+3,rgby,1)=80;
                end
            
				if imgSlope2(rgbx,rgby+1,1)==0 	imgSlope2(rgbx,rgby+1,1)=80;
                end
                if imgSlope2(rgbx+1,rgby+1,1)==0 imgSlope2(rgbx+1,rgby+1,1)=80;
                end
                if imgSlope2(rgbx+2,rgby+1,1)==0 imgSlope2(rgbx+2,rgby+1,1)=80;
                end
                if imgSlope2(rgbx+3,rgby+1,1)==0 imgSlope2(rgbx+3,rgby+1,1)=80;
                end
              
				if imgSlope2(rgbx,rgby+2,1)==0 	imgSlope2(rgbx,rgby+2,1)=80;
                end
                if imgSlope2(rgbx+1,rgby+2,1)==0 imgSlope2(rgbx+1,rgby+2,1)=80;
                end
                if imgSlope2(rgbx+2,rgby+2,1)==0 imgSlope2(rgbx+2,rgby+2,1)=80;
                end
                if imgSlope2(rgbx+3,rgby+2,1)==0 imgSlope2(rgbx+3,rgby+2,1)=80;
                end
            
				if imgSlope2(rgbx,rgby+3,1)==0 	imgSlope2(rgbx,rgby+3,1)=80;
                end
                if imgSlope2(rgbx+1,rgby+3,1)==0 imgSlope2(rgbx+1,rgby+3,1)=80;
                end
                if imgSlope2(rgbx+2,rgby+3,1)==0 imgSlope2(rgbx+2,rgby+3,1)=80;
                end
                if imgSlope2(rgbx+3,rgby+3,1)==0 imgSlope2(rgbx+3,rgby+3,1)=80;
                end
                
				if imgSlope2(rgbx,rgby,2)==0 	imgSlope2(rgbx,rgby,2)=140;
                end
                if imgSlope2(rgbx+1,rgby,2)==0 imgSlope2(rgbx+1,rgby,2)=140;
                end
                if imgSlope2(rgbx+2,rgby,2)==0 imgSlope2(rgbx+2,rgby,2)=140;
                end
                if imgSlope2(rgbx+3,rgby,2)==0 imgSlope2(rgbx+3,rgby,2)=140;
                end
            
				if imgSlope2(rgbx,rgby+1,2)==0 	imgSlope2(rgbx,rgby+1,2)=140;
                end
                if imgSlope2(rgbx+1,rgby+1,2)==0 imgSlope2(rgbx+1,rgby+1,2)=140;
                end
                if imgSlope2(rgbx+2,rgby+1,2)==0 imgSlope2(rgbx+2,rgby+1,2)=140;
                end
                if imgSlope2(rgbx+3,rgby+1,2)==0 imgSlope2(rgbx+3,rgby+1,2)=140;
                end
              
				if imgSlope2(rgbx,rgby+2,2)==0 	imgSlope2(rgbx,rgby+2,2)=140;
                end
                if imgSlope2(rgbx+1,rgby+2,2)==0 imgSlope2(rgbx+1,rgby+2,2)=140;
                end
                if imgSlope2(rgbx+2,rgby+2,2)==0 imgSlope2(rgbx+2,rgby+2,2)=140;
                end
                if imgSlope2(rgbx+3,rgby+2,2)==0 imgSlope2(rgbx+3,rgby+2,2)=140;
                end
            
				if imgSlope2(rgbx,rgby+3,2)==0 	imgSlope2(rgbx,rgby+3,2)=140;
                end
                if imgSlope2(rgbx+1,rgby+3,2)==0 imgSlope2(rgbx+1,rgby+3,2)=140;
                end
                if imgSlope2(rgbx+2,rgby+3,2)==0 imgSlope2(rgbx+2,rgby+3,2)=140;
                end
                if imgSlope2(rgbx+3,rgby+3,2)==0 imgSlope2(rgbx+3,rgby+3,2)=140;
                end
               
				if imgSlope2(rgbx,rgby,3)==0 	imgSlope2(rgbx,rgby,3)=40;
                end
                if imgSlope2(rgbx+1,rgby,3)==0 imgSlope2(rgbx+1,rgby,3)=40;
                end
                if imgSlope2(rgbx+2,rgby,3)==0 imgSlope2(rgbx+2,rgby,3)=40;
                end
                if imgSlope2(rgbx+3,rgby,3)==0 imgSlope2(rgbx+3,rgby,3)=40;
                end
            
				if imgSlope2(rgbx,rgby+1,3)==0 	imgSlope2(rgbx,rgby+1,3)=40;
                end
                if imgSlope2(rgbx+1,rgby+1,3)==0 imgSlope2(rgbx+1,rgby+1,3)=40;
                end
                if imgSlope2(rgbx+2,rgby+1,3)==0 imgSlope2(rgbx+2,rgby+1,3)=40;
                end
                if imgSlope2(rgbx+3,rgby+1,3)==0 imgSlope2(rgbx+3,rgby+1,3)=40;
                end
              
				if imgSlope2(rgbx,rgby+2,3)==0 	imgSlope2(rgbx,rgby+2,3)=40;
                end
                if imgSlope2(rgbx+1,rgby+2,3)==0 imgSlope2(rgbx+1,rgby+2,3)=40;
                end
                if imgSlope2(rgbx+2,rgby+2,3)==0 imgSlope2(rgbx+2,rgby+2,3)=40;
                end
                if imgSlope2(rgbx+3,rgby+2,3)==0 imgSlope2(rgbx+3,rgby+2,3)=40;
                end
            
				if imgSlope2(rgbx,rgby+3,3)==0 	imgSlope2(rgbx,rgby+3,3)=40;
                end
                if imgSlope2(rgbx+1,rgby+3,3)==0 imgSlope2(rgbx+1,rgby+3,3)=40;
                end
                if imgSlope2(rgbx+2,rgby+3,3)==0 imgSlope2(rgbx+2,rgby+3,3)=40;
                end
                if imgSlope2(rgbx+3,rgby+3,3)==0 imgSlope2(rgbx+3,rgby+3,3)=40;
                end
		end
		
    end
end

imshow(imgSlope2);

