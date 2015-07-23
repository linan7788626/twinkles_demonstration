#cython: language_level=3,boundscheck=False, wraparound=False,nonecheck=False
#import cython
import numpy as np
cimport numpy as np

ctypedef np.float64_t Dtype
#ctypedef double Dtype

#-------------------------------------------------------------

cdef extern from "./lensing_funcs.h":
    void sdens_to_kappa(double p_mass_in, double* sdens_in, int Ncc, double Dcell, double zl, double zs, double *kappa)
    void sdens_to_kappac(double p_mass_in, double* sdens_in, int Ncc, double Dcell, double zl, double zs, double *kappac)
    void kappa_to_phi(double *kappa, double *phi,int Ncc2,double Dcell)
    void kappa_to_alphas(double *kappa,double *alpha1,double *alpha2,int Ncc2,double Dcell)
    void kappa_to_shears(double *kappa,double *shear1,double *shear2, int Ncc2,double Dcell)
    void calculate_mu(double *kappa,double *shear1,double *shear2, int Ncc, double *mu)
    void calculate_td(double *phi,double *alpha1,double *alpha2, int Ncc, double *td)

def call_all_about_lensing(np.ndarray[Dtype, ndim=2, mode="c"] sdens,
                           int Nc,double zl,double zs,double p_mass_in,double dsx):

    cdef int Nc2 = Nc*2
    cdef np.ndarray kappa = np.zeros((Nc2,Nc2),dtype=np.double)
    sdens_to_kappa(p_mass_in,<Dtype *>sdens.data,Nc,dsx,zl,zs,<Dtype *>kappa.data)

    cdef np.ndarray kappac = np.zeros((Nc,Nc),dtype=np.double)
    sdens_to_kappac(p_mass_in,<Dtype *>sdens.data,Nc,dsx,zl,zs,<Dtype *>kappac.data)
#--------------------------------------------------------------------------------

    cdef np.ndarray phi = np.zeros((Nc,Nc),dtype=np.double)
    kappa_to_phi(<Dtype *>kappa.data, <Dtype *>phi.data,Nc2,dsx)

    cdef np.ndarray alpha1 = np.zeros((Nc,Nc),dtype=np.double)
    cdef np.ndarray alpha2 = np.zeros((Nc,Nc),dtype=np.double)
    kappa_to_alphas(<Dtype *>kappa.data,<Dtype *>alpha1.data,<Dtype *>alpha2.data,Nc2,dsx)

    cdef np.ndarray td = np.zeros((Nc,Nc),dtype=np.double)
    calculate_td(<Dtype *>phi.data,<Dtype *>alpha1.data,<Dtype *>alpha2.data,Nc,<Dtype *>td.data)
#--------------------------------------------------------------------------------

    cdef np.ndarray shear1 = np.zeros((Nc,Nc),dtype=np.double)
    cdef np.ndarray shear2 = np.zeros((Nc,Nc),dtype=np.double)
    kappa_to_shears(<Dtype *>kappa.data,<Dtype *>shear1.data,<Dtype *>shear2.data,Nc2,dsx)

    cdef np.ndarray mu = np.zeros((Nc,Nc),dtype=np.double)
    calculate_mu(<Dtype *>kappac.data,<Dtype *>shear1.data,<Dtype *>shear2.data,Nc,<Dtype *>mu.data)

    return phi,alpha1,alpha2,td,mu,kappac