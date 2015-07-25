#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include "mycosmology.h"
//----------------------------------------------------------------------------------
void Loadin_grids_mesh(double boxsize, double xc1, double xc2, int Ncc, double *posx1, double *posx2) {
	double dsx = boxsize/(double)Ncc;

	int i,j;
	int index;
	for (i=0; i<Ncc; i++) for (j=0; j<Ncc; j++) {
		index = i*Ncc+j;
		posx1[index] = dsx*(double)(i)-boxsize*0.5+0.5*dsx+xc1;
		posx2[index] = dsx*(double)(j)-boxsize*0.5+0.5*dsx+xc2;
	}
}
//----------------------------------------------------------------------------------
void sdens_to_kappa(double p_mass_in, double* sdens_in, int Ncc, double Dcell, double zl, double zs, double *kappa) {

	int Nc2 = Ncc*2;
	double scrit;
	scrit = sigma_crit(zl,zs);//*(apr*apr/(Da(zl)*Da(zl)));
	//double kappa_max = 0.0;

	int i,j,index1,index2;
	for (i=Nc2/4;i<Nc2*3/4;i++) for (j=Nc2/4;j<Nc2*3/4;j++) {
		index1 = i*Nc2+j;
		index2 = (i-Nc2/4)*Nc2/2+(j-Nc2/4);
		kappa[index1] = p_mass_in*sdens_in[index2];///scrit;
	}
}
//----------------------------------------------------------------------------------
void sdens_to_kappac(double p_mass_in, double* sdens_in, int Ncc, double Dcell, double zl, double zs, double *kappac) {

	double scrit;
	scrit = sigma_crit(zl,zs);//*(apr*apr/(Da(zl)*Da(zl)));
	//double kappa_max = 0.0;

	int i,j,index;
	for (i=0;i<Ncc;i++) for (j=0;j<Ncc;j++) {
		index = i*Ncc+j;
		kappac[index] = p_mass_in*sdens_in[index];///scrit;
	}
}
////--------------------------------------------------------------------
//void kernel_green_iso(int Ncc, double *in, double Dcell) {
//	int i,j;
//	double x,y,r;
//	double epsilon = sqrt(2.0)*Dcell;
//
//	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
//		if(i <=(Ncc/2)  && j <=(Ncc/2)) {
//			x = (double)(i)*Dcell;
//			y = (double)(j)*Dcell;
//			r = sqrt(x*x+y*y+epsilon*epsilon);
//
//			in[i*Ncc+j] = 1.0/M_PI*log(r);
//		}
//		else {
//			if(i <= Ncc/2 && j > (Ncc/2)) {
//				in[i*Ncc+j] = in[i*Ncc+Ncc+1-j];
//			}
//			if(i > (Ncc/2) && j <= (Ncc/2)) {
//				in[i*Ncc+j] = in[(Ncc+1-i)*Ncc+j];
//			}
//
//			if(i > (Ncc/2) && j > (Ncc/2)) {
//				in[i*Ncc+j] = in[(Ncc+1-i)*Ncc+Ncc+1-j];
//			}
//		}
//	}
//}
//--------------------------------------------------------------------
void kernel_green_iso(int Ncc, double *in, double Dcell) {
	int i,j;
	double x,y,r;
	double epsilon = sqrt(2.0)*Dcell;

	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
		if(i <(Ncc/2)  && j <(Ncc/2)) {
			x = (double)(i)*Dcell;
			y = (double)(j)*Dcell;
			r = sqrt(x*x+y*y+epsilon*epsilon);

			in[i*Ncc+j] = 1.0/M_PI*log(r);
		}
		else {
			if(i < Ncc/2 && j >= (Ncc/2)) {
				in[i*Ncc+j] = in[i*Ncc+(Ncc-j)];
			}
			if(i >= (Ncc/2) && j < (Ncc/2)) {
				in[i*Ncc+j] = in[(Ncc-i)*Ncc+j];
			}

			if(i >= (Ncc/2) && j >= (Ncc/2)) {
				in[i*Ncc+j] = in[(Ncc-i)*Ncc+(Ncc-j)];
			}
		}
	}
}
//--------------------------------------------------------------------
void kernel_shears_iso(int Ncc,double *in1,double *in2,double Dcell) {
	int i,j;
	double x,y,r;
	double epsilon = sqrt(2.0)*Dcell;

	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
		if(i <(Ncc/2)  && j <(Ncc/2)) {
			x = (double)(i)*Dcell;
			y = (double)(j)*Dcell;
			r = sqrt(x*x+y*y+epsilon*epsilon);

			in1[i*Ncc+j] =  (y*y-x*x)/(M_PI*r*r*r*r);
			in2[i*Ncc+j] = (-2.0*x*y)/(M_PI*r*r*r*r);
		}

		else {
			if(i < Ncc/2 && j >= (Ncc/2)) {
				in1[i*Ncc+j]  =  in1[i*Ncc+(Ncc-j)];
				in2[i*Ncc+j]  = -in2[i*Ncc+(Ncc-j)];
			}
			if(i >= (Ncc/2) && j < (Ncc/2)) {
				in1[i*Ncc+j]  =  in1[(Ncc-i)*Ncc+j];
				in2[i*Ncc+j]  = -in2[(Ncc-i)*Ncc+j];
			}

			if(i >= (Ncc/2) && j >= (Ncc/2)) {
				in1[i*Ncc+j]  =  in1[(Ncc-i)*Ncc+(Ncc-j)];
				in2[i*Ncc+j]  =  in2[(Ncc-i)*Ncc+(Ncc-j)];
			}
		}
	}
}
//--------------------------------------------------------------------
void kernel_alphas_iso(int Ncc,double *in1,double *in2,double Dcell) {
	int i,j;
	double x,y,r;
	double epsilon = sqrt(2.0)*Dcell*0.00000000001;

	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
		if(i <(Ncc/2)  && j <(Ncc/2)) {
			x = (double)(i)*Dcell;
			y = (double)(j)*Dcell;
			r = sqrt(x*x+y*y+epsilon*epsilon);

			in1[i*Ncc+j] = x/(M_PI*r*r);
			in2[i*Ncc+j] = y/(M_PI*r*r);
		}
		else {
			if(i < Ncc/2 && j >= (Ncc/2)) {
				in1[i*Ncc+j]  =  in1[i*Ncc+(Ncc-j)];
				in2[i*Ncc+j]  = -in2[i*Ncc+(Ncc-j)];
			}
			if(i >= (Ncc/2) && j < (Ncc/2)) {
				in1[i*Ncc+j]  = -in1[(Ncc-i)*Ncc+j];
				in2[i*Ncc+j]  =  in2[(Ncc-i)*Ncc+j];
			}

			if(i >= (Ncc/2) && j >= (Ncc/2)) {
				in1[i*Ncc+j]  = -in1[(Ncc-i)*Ncc+(Ncc-j)];
				in2[i*Ncc+j]  = -in2[(Ncc-i)*Ncc+(Ncc-j)];
			}
		}
	}
}
//--------------------------------------------------------------------
void kernel_smooth_iso(double sigma,int Ncc,double *in,double Dcell) {
	int i,j;
	double x,y,r;
	double epsilon = 0.00000001*Dcell;
	double cnorm = 0.0;

	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
		if(i <(Ncc/2)  && j <(Ncc/2)) {
			x = (double)(i)*Dcell-0.5*Dcell;
			y = (double)(j)*Dcell-0.5*Dcell;
			r = sqrt(x*x+y*y+epsilon*epsilon);

			in[i*Ncc+j] = 1.0/(2.0*M_PI*sigma*sigma)*exp(-(r*r)/(2.0*sigma*sigma));
		}
		else {
			if(i < Ncc/2 && j >= (Ncc/2)) {
				in[i*Ncc+j] = in[i*Ncc+(Ncc-j)];
			}
			if(i >= (Ncc/2) && j < (Ncc/2)) {
				in[i*Ncc+j] = in[(Ncc-i)*Ncc+j];
			}

			if(i >= (Ncc/2) && j >= (Ncc/2)) {
				in[i*Ncc+j] = in[(Ncc-i)*Ncc+(Ncc-j)];
			}
		}
		cnorm += in[i*Ncc+j]*Dcell*Dcell;
	}

	double ctotal = 0.0;
	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
		in[i*Ncc+j] = in[i*Ncc+j]/cnorm;
		ctotal += in[i*Ncc+j]*Dcell*Dcell;
	}
}
//--------------------------------------------------------------------
////--------------------------------------------------------------------
//void kernel_shears_iso(int Ncc,double *in1,double *in2,double Dcell) {
//	int i,j;
//	double x,y,r;
//	double epsilon = sqrt(2.0)*Dcell;
//
//	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
//		if(i <=(Ncc/2)  && j <=(Ncc/2)) {
//			x = (double)(i)*Dcell;
//			y = (double)(j)*Dcell;
//			r = sqrt(x*x+y*y+epsilon*epsilon);
//
//			in1[i*Ncc+j] =  (y*y-x*x)/(M_PI*r*r*r*r);
//			in2[i*Ncc+j] = (-2.0*x*y)/(M_PI*r*r*r*r);
//		}
//
//		else {
//			if(i <= Ncc/2 && j > (Ncc/2)) {
//				in1[i*Ncc+j]  =  in1[i*Ncc+Ncc+1-j];
//				in2[i*Ncc+j]  = -in2[i*Ncc+Ncc+1-j];
//			}
//			if(i > (Ncc/2) && j <= (Ncc/2)) {
//				in1[i*Ncc+j]  =  in1[(Ncc+1-i)*Ncc+j];
//				in2[i*Ncc+j]  = -in2[(Ncc+1-i)*Ncc+j];
//			}
//
//			if(i > (Ncc/2) && j > (Ncc/2)) {
//				in1[i*Ncc+j]  =  in1[(Ncc+1-i)*Ncc+Ncc+1-j];
//				in2[i*Ncc+j]  =  in2[(Ncc+1-i)*Ncc+Ncc+1-j];
//			}
//		}
//	}
//}
////--------------------------------------------------------------------
//void kernel_alphas_iso(int Ncc,double *in1,double *in2,double Dcell) {
//	int i,j;
//	double x,y,r;
//	double epsilon = sqrt(2.0)*Dcell;
//
//	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
//		if(i <=(Ncc/2)  && j <=(Ncc/2)) {
//			x = (double)(i)*Dcell;
//			y = (double)(j)*Dcell;
//			r = sqrt(x*x+y*y+epsilon*epsilon);
//
//			in1[i*Ncc+j] = x/(M_PI*r*r);
//			in2[i*Ncc+j] = y/(M_PI*r*r);
//		}
//		else {
//			if(i <= Ncc/2 && j > (Ncc/2)) {
//				in1[i*Ncc+j]  =  in1[i*Ncc+Ncc+1-j];
//				in2[i*Ncc+j]  = -in2[i*Ncc+Ncc+1-j];
//			}
//			if(i > (Ncc/2) && j <= (Ncc/2)) {
//				in1[i*Ncc+j]  = -in1[(Ncc+1-i)*Ncc+j];
//				in2[i*Ncc+j]  =  in2[(Ncc+1-i)*Ncc+j];
//			}
//
//			if(i > (Ncc/2) && j > (Ncc/2)) {
//				in1[i*Ncc+j]  = -in1[(Ncc+1-i)*Ncc+Ncc+1-j];
//				in2[i*Ncc+j]  = -in2[(Ncc+1-i)*Ncc+Ncc+1-j];
//			}
//		}
//	}
//}
////--------------------------------------------------------------------
//void kernel_smooth_iso(double sigma,int Ncc,double *in,double Dcell) {
//	int i,j;
//	double x,y,r;
//	double epsilon = 0.00000001*Dcell;
//	double cnorm = 0.0;
//
//	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
//		if(i <=(Ncc/2)  && j <=(Ncc/2)) {
//			x = (double)(i)*Dcell-0.5*Dcell;
//			y = (double)(j)*Dcell-0.5*Dcell;
//			r = sqrt(x*x+y*y+epsilon*epsilon);
//
//			in[i*Ncc+j] = 1.0/(2.0*M_PI*sigma*sigma)*exp(-(r*r)/(2.0*sigma*sigma));
//		}
//		else {
//			if(i <= Ncc/2 && j > (Ncc/2)) {
//				in[i*Ncc+j] = in[i*Ncc+Ncc+1-j];
//			}
//			if(i > (Ncc/2) && j <= (Ncc/2)) {
//				in[i*Ncc+j] = in[(Ncc+1-i)*Ncc+j];
//			}
//
//			if(i > (Ncc/2) && j > (Ncc/2)) {
//				in[i*Ncc+j] = in[(Ncc+1-i)*Ncc+Ncc+1-j];
//			}
//		}
//		cnorm += in[i*Ncc+j]*Dcell*Dcell;
//	}
//
//	double ctotal = 0.0;
//	for(i=0;i<Ncc;i++) for(j=0;j<Ncc;j++) {
//		in[i*Ncc+j] = in[i*Ncc+j]/cnorm;
//		ctotal += in[i*Ncc+j]*Dcell*Dcell;
//	}
//}
////--------------------------------------------------------------------
void fftw_r2c_2d(double *in_real, fftw_complex *in_fft, long Ncell, double Dcell) {
	long i,j;
	long nh = Ncell/2+1;
	long index;

	fftw_complex *in_fft_tmp = (fftw_complex *)fftw_malloc(Ncell*nh*sizeof(fftw_complex));
	fftw_plan plfd;
	plfd = fftw_plan_dft_r2c_2d(Ncell,Ncell,in_real,in_fft_tmp,FFTW_ESTIMATE);
	fftw_execute(plfd);

	for(i=0;i<Ncell;i++) for(j=0;j<nh;j++) {
		index = i*nh+j;
		in_fft[index][0] = in_fft_tmp[index][0];
		in_fft[index][1] = in_fft_tmp[index][1];
	}

	fftw_destroy_plan(plfd);
	fftw_free(in_fft_tmp);
}
//--------------------------------------------------------------------
void fftw_c2r_2d(fftw_complex *in_fft, double *in_real, long Ncell, double Dcell) {

	int i,j;
	//int nh = Ncell/2+1;
	int index;

	double *in_real_tmp = (double *)malloc(Ncell*Ncell*sizeof(double));
	fftw_plan plbd;
	plbd = fftw_plan_dft_c2r_2d(Ncell, Ncell, in_fft, in_real_tmp, FFTW_ESTIMATE);
	fftw_execute(plbd);

	for(i=0;i<Ncell;i++) for(j=0;j<Ncell;j++) {
		index = i*Ncell+j;
		in_real[index] = in_real_tmp[index];
	}

	fftw_destroy_plan(plbd);
	free(in_real_tmp);
}
//--------------------------------------------------------------------
void convolve_fft(double *in1, double *in2, double *out, long Ncell, double Dcell) {
	int i,j;
	int nh = Ncell/2+1;
	int index;
	double tmpr,tmpi;

	double in1_fftr = 0.0;
	double in1_ffti = 0.0;
	double in2_fftr = 0.0;
	double in2_ffti = 0.0;

	fftw_complex *in1_fft = (fftw_complex *)fftw_malloc(Ncell*nh*sizeof(fftw_complex));
	fftw_complex *in2_fft = (fftw_complex *)fftw_malloc(Ncell*nh*sizeof(fftw_complex));

	fftw_r2c_2d(in1, in1_fft, Ncell, Dcell);
	fftw_r2c_2d(in2, in2_fft, Ncell, Dcell);

	fftw_complex *out_fft = (fftw_complex *)fftw_malloc(Ncell*nh*sizeof(fftw_complex));
	for (i=0;i<Ncell;i++) for(j=0;j<nh;j++) {
		index = i*nh+j;
		in1_fftr = in1_fft[index][0];
		in1_ffti = in1_fft[index][1];

		in2_fftr = in2_fft[index][0];
		in2_ffti = in2_fft[index][1];
		tmpr = in1_fftr*in2_fftr-in1_ffti*in2_ffti;
		tmpi = in1_fftr*in2_ffti+in1_ffti*in2_fftr;
		out_fft[index][0] = tmpr;
		out_fft[index][1] = tmpi;
	}


	double *out_tmp = (double *)malloc(Ncell*Ncell*sizeof(double));
	fftw_c2r_2d(out_fft, out_tmp, Ncell, Dcell);

	for(i=0;i<Ncell;i++) for(j=0;j<Ncell;j++) {
		index = i*Ncell+j;
		out[index] = out_tmp[index]/(Ncell*Ncell)*Dcell*Dcell;
	}

	fftw_free(in1_fft);
	fftw_free(in2_fft);
	fftw_free(out_fft);

	free(out_tmp);
}
//--------------------------------------------------------------------
void kappa_to_phi(double *kappa, double *phi,int Ncc2,double Dcell) {

	double *green_iso = calloc(Ncc2*Ncc2,sizeof(double));

	kernel_green_iso(Ncc2,green_iso,Dcell);

	double *phi_tmp = malloc(Ncc2*Ncc2*sizeof(double));

	convolve_fft(kappa,green_iso,phi_tmp,Ncc2,Dcell);

	int i,j,index1,index2;
	for (i=Ncc2/4;i<Ncc2*3/4;i++) for (j=Ncc2/4;j<Ncc2*3/4;j++) {
		index1 = i*Ncc2+j;
		index2 = (i-Ncc2/4)*Ncc2/2+(j-Ncc2/4);
		phi[index2] = phi_tmp[index1];
	}

	free(phi_tmp);
}
//--------------------------------------------------------------------
void kappa_to_alphas(double *kappa,double *alpha1,double *alpha2,int Ncc2,double Dcell) {

	double *alpha1_iso = calloc(Ncc2*Ncc2,sizeof(double));
	double *alpha2_iso = calloc(Ncc2*Ncc2,sizeof(double));

	kernel_alphas_iso(Ncc2,alpha1_iso,alpha2_iso,Dcell);

	double *alpha1_tmp = malloc(Ncc2*Ncc2*sizeof(double));
	double *alpha2_tmp = malloc(Ncc2*Ncc2*sizeof(double));

	convolve_fft(kappa,alpha1_iso,alpha1_tmp,Ncc2,Dcell);
	convolve_fft(kappa,alpha2_iso,alpha2_tmp,Ncc2,Dcell);

	int i,j,index1,index2;
	for (i=Ncc2/4;i<Ncc2*3/4;i++) for (j=Ncc2/4;j<Ncc2*3/4;j++) {
		index1 = i*Ncc2+j;
		index2 = (i-Ncc2/4)*Ncc2/2+(j-Ncc2/4);
		alpha1[index2] = alpha1_tmp[index1];
		alpha2[index2] = alpha2_tmp[index1];
	}

	free(alpha1_tmp);
	free(alpha2_tmp);
}
//--------------------------------------------------------------------
void kappa_to_shears(double *kappa,double *shear1,double *shear2, int Ncc2,double Dcell) {

	double *shear1_iso = malloc(Ncc2*Ncc2*sizeof(double));
	double *shear2_iso = malloc(Ncc2*Ncc2*sizeof(double));

	kernel_shears_iso(Ncc2,shear1_iso,shear2_iso,Dcell);

	double *shear1_tmp = malloc(Ncc2*Ncc2*sizeof(double));
	double *shear2_tmp = malloc(Ncc2*Ncc2*sizeof(double));

	convolve_fft(kappa,shear1_iso,shear1_tmp,Ncc2,Dcell);
	convolve_fft(kappa,shear2_iso,shear2_tmp,Ncc2,Dcell);

	int i,j,index1,index2;
	for (i=Ncc2/4;i<Ncc2*3/4;i++) for (j=Ncc2/4;j<Ncc2*3/4;j++) {
		index1 = i*Ncc2+j;
		index2 = (i-Ncc2/4)*Ncc2/2+(j-Ncc2/4);
		shear1[index2] = shear1_tmp[index1];
		shear2[index2] = shear2_tmp[index1];
	}

	free(shear1_tmp);
	free(shear2_tmp);
	free(shear1_iso);
	free(shear2_iso);
}
//--------------------------------------------------------------------
void calculate_mu(double *kappac,double *shear1,double *shear2, int Ncc, double *mu) {

	int i,j,index;
	for (i=0;i<Ncc;i++) for (j=0;j<Ncc;j++) {
		index = i*Ncc+j;
		mu[index] = 1.0/((1.0-kappac[index])*(1.0-kappac[index])
				-shear1[index]*shear1[index]-shear2[index]*shear2[index]);
	}
}
//--------------------------------------------------------------------
void calculate_td(double *phi,double *alpha1,double *alpha2, int Ncc, double *td) {
	double Kc = 1.0;
	//Kc = (1.0+zl)/c*(Dl*Ds/Dls)
	//td = Kc*(0.5*((al1)**2.0+(al2)**2.0)-phi)
	//td = Kc*(0.5*((x1-ys1)**2.0+(x2-ys2)**2.0)-phi)

	int i,j,index;
	for (i=0;i<Ncc;i++) for (j=0;j<Ncc;j++) {
		index = i*Ncc+j;
		td[index] = Kc*((alpha1[index]*alpha1[index]+alpha2[index]*alpha2[index])-phi[index]);
	}
}
