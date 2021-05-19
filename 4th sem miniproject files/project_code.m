clc; clear all;
close all;
fname= 'cameraman.tif';
[im,map] = imread(fname);
im=imresize(im,[512 512]);
% im=rgb2gray(im);
subplot(2,3,1);
imshow(im);
title('original image');
[N,M]=size(im);

% percentage of noise (salt & pepper)  to add into the input image 
noisep =20/100; 
nim=imnoise(im,'salt & pepper',noisep);
subplot(2,3,2);
imshow(nim);
title('noised image'); 


%mean filter
h=fspecial('average',3);
filter_image_mean=imfilter(nim,h);
 subplot(2,3,4);
imshow(filter_image_mean);
title('filter-image-mean');

L=M*N;
%  mean square error between original and denoised  image
mse1=(1/L)* sum(sum((im-filter_image_mean).^2));
%  peak to signal ratio between original and denoised  image
psnr1=20.0* (log10(255/sqrt(mse1)));
disp('MEAN FILTER');
disp('PSNR='); disp(psnr1);
disp('MSE='); disp(mse1);

%median filter
filter_image_median=medfilt2(nim);
 subplot(2,3,5);
imshow(filter_image_median);
title('filter-image-median');
%  mean square error between original and denoised  image
mse2=(1/L)* sum(sum((im-filter_image_median).^2));
%  peak to signal ratio between original and denoised  image
psnr2=20.0* (log10(255/sqrt(mse2)));
disp('MEDIAN FILTER');
disp('PSNR='); disp(psnr2);
disp('MSE='); disp(mse2);


%adaptive median filter

% initialize denoised image matrix with zeros elements
dim=zeros(M,N);

% set maximum median filter window size
wsmax=21; 

for m=1 : M
  for n=1 : N  

    % set minimum median filter window size
    ws =3; 
    while ( ws < wsmax )
     
      %  neighbouring subscripts of centre pixel           
     hws = floor(ws/2);
     [J,I]=meshgrid(n-hws:n+hws ,m-hws:m+hws);
     I=I(:); J=J(:);          

      % exclude subscripts out of border of the image 
      if ( m<=hws || n<=hws || m>=M-hws || n>=N-hws)        
            s =(I>=1 & I<=M);
            I=I(s); J=J(s);      
            s =(J>=1 & J<=M);
            I=I(s); J=J(s);       
      end      

     %  converts neighbouring subscripts index  
      ind = sub2ind([M,N],I,J);          
       
     %  minimum of neighbouring pixels  
      S1 = min ( nim(ind)  ) ;
     %  median of neighbouring pixels  
      S2 =  median ( nim(ind)  );
     %  maximum of neighbouring pixels  
      S3 =  max( nim(ind) );
       
       if ( S1<S2 && S2<S3 )
           
           if ( nim(m,n)>S1 && nim(m,n)<S3 ) 
                 dim(m,n) = nim(m,n);                
           else
                dim(m,n) = S2; 
           end
           break;
           
       else  
           % increment  median filter window size by 2         
           ws = ws+2;           

            %  if max window size reached, set S2 as denoised pixel and 
              % break the while-loop          
           if (  ws > wsmax )
                  dim(m,n) = S2;
                  break;
           end           
       end
             
    end
  end
end
dim=uint8(dim);
%  mean square error between original and denoised  image
mse3=(1/L)* sum(sum((im-dim).^2));
%  peak to signal ratio between original and denoised  image
psnr3=20.0* (log10(255/sqrt(mse3)));
subplot(2,3,6);
imshow(dim);
title('filter-image-adaptivemedian');
disp('ADAPTIVE MEDIAN FILTER');
disp('PSNR='); disp(psnr3);
disp('MSE='); disp(mse3);


