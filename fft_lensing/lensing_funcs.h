#include <fftw3.h>
void Loadin_grids_mesh(double boxsize, double xc1, double xc2, int Ncc, double *posx1, double *posx2);
void sdens_to_kappa(double p_mass_in, double* sdens_in, int Ncc, double Dcell, double zl, double zs, double *kappa);
void sdens_to_kappac(double p_mass_in, double* sdens_in, int Ncc, double Dcell, double zl, double zs, double *kappac);
void kernel_green_iso(int Ncc, double *in, double Dcell);
void kernel_shears_iso(int Ncc,double *in1,double *in2,double Dcell);
void kernel_alphas_iso(int Ncc,double *in1,double *in2,double Dcell);
void kernel_smooth_iso(double sigma,int Ncc,double *in,double Dcell);
void fftw_r2c_2d(double *in_real, fftw_complex *in_fft, long Ncell, double Dcell);
void fftw_c2r_2d(fftw_complex *in_fft, double *in_real, long Ncell, double Dcell);
void convolve_fft(double *in1, double *in2, double *out, long Ncell, double Dcell);
void kappa_to_phi(double *kappa, double *phi,int Ncc2,double Dcell);
void kappa_to_alphas(double *kappa,double *alpha1,double *alpha2,int Ncc2,double Dcell);
void kappa_to_shears(double *kappa,double *shear1,double *shear2, int Ncc2,double Dcell);
void sdens_to_phi(double * sdens, int Nc, double boxsize, double zl, double zs, double * phi);
void sdens_to_alphas(double * sdens, int Nc, double boxsize, double zl, double zs, double * alpha1, double * alpha2);
void sdens_to_shears(double * sdens, int Nc, double boxsize, double zl, double zs, double * shear1, double * shear2);
void calculate_mu(double *kappa,double *shear1,double *shear2, int Ncc, double *mu);
void calculate_td(double *phi,double *alpha1,double *alpha2, int Ncc, double *td);