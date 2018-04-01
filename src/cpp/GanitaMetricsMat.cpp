#include "ganita/metrics/GanitaMetricsMat.hpp"

GanitaMetricsMat::GanitaMetricsMat(void)
{
  gmmat = 0;
  ncols = 0;
  nrows = 0;
}

GanitaMetricsMat::GanitaMetricsMat(unsigned long nn, unsigned long mm)
{
  unsigned long ii;
  ncols = nn; nrows = mm;
  gmmat = new double *[nn] ;
  //memory allocated for elements of each column.
  
  for( ii = 0 ; ii < nn; ii++ )
    gmmat[ii] = new double[mm]();
}

int GanitaMetricsMat::close(void)
{
  //free the allocated memory
  
  for( unsigned long i = 0 ; i < ncols ; i++ )
    delete [] gmmat[i] ;
  delete [] gmmat ;
  return(1);
}

unsigned long GanitaMetricsMat::returnNRows(void)
{
  return(nrows);
}

unsigned long GanitaMetricsMat::returnNCols(void)
{
  return(ncols);
}

int GanitaMetricsMat::set(unsigned long cc, unsigned long rr, double value)
{
  if(cc >= ncols) cc = cc % ncols;
  if(rr >= nrows) rr = rr % nrows;
  gmmat[cc][rr] = value;
  return(1);
}

double GanitaMetricsMat::get(unsigned long cc, unsigned long rr)
{
  if(cc >= ncols) cc = cc % ncols;
  if(rr >= nrows) rr = rr % nrows;
  return(gmmat[cc][rr]);
}

int GanitaMetricsMat::leftMultiply(GanitaMetricsMat mymat)
{
  if(ncols != mymat.returnNRows()){
    // cannot multiply
    return(-1);
  }
  unsigned long ii, jj, kk;
  unsigned long nc;
  double sum;
  GanitaMetricsMat acol(1,nrows);
  nc = mymat.returnNCols();
  for(jj=0; jj<nc; jj++){
    for(ii=0; ii<nrows; ii++){
      sum = 0;
      for(kk=0; kk<ncols; kk++){
	sum += gmmat[kk][ii] * mymat.get(jj,kk);
      }
      acol.set(0,ii,sum);
    }
    for(ii=0; ii<nrows; ii++){
      mymat.set(jj,ii,acol.get(0,ii));
    }
  }
  acol.close();
  return(1);
}

int GanitaMetricsMat::leftMultiply(GanitaMetricsMat mymat, GanitaMetricsMat result)
{
  if(ncols != mymat.returnNRows()){
    // cannot multiply
    return(-1);
  }
  unsigned long ii, jj, kk;
  unsigned long nc;
  double sum;
  nc = mymat.returnNCols();
  //result.init(nrows,mymat.returnNCols());
  for(jj=0; jj<nc; jj++){
    for(ii=0; ii<nrows; ii++){
      sum = 0;
      for(kk=0; kk<ncols; kk++){
	sum += gmmat[kk][ii] * mymat.get(jj,kk);
      }
      result.set(jj,ii,sum);
    }
  }
  return(1);
}

int GanitaMetricsMat::square(void)
{
  if(ncols != nrows){
    // cannot multiply
    return(-1);
  }
  unsigned long ii, jj, kk;
  double sum;
  GanitaMetricsMat tmat(ncols,nrows);
  for(jj=0; jj<ncols; jj++){
    for(ii=0; ii<nrows; ii++){
      sum = 0;
      for(kk=0; kk<ncols; kk++){
	sum += gmmat[kk][ii] * gmmat[jj][kk];
      }
      tmat.set(jj,ii,sum);
    }
  }
  for(jj=0; jj<ncols; jj++){
    for(ii=0; ii<nrows; ii++){
      gmmat[jj][ii] = tmat.get(jj,ii);
    }
  }
  tmat.close();

  return(1);
}

int GanitaMetricsMat::dumpMat(void)
{
  unsigned long ii, jj;
  for(ii=0; ii<nrows; ii++){
      for(jj=0; jj<ncols; jj++){
	fprintf(stdout, "%lf ", gmmat[jj][ii]);
      }
      fprintf(stdout, "\n");
  }
  return(1);
}

int GanitaMetricsMat::setEx1(void)
{
  // gmmat[1][0] = 1;
  // gmmat[2][1] = 1;
  // gmmat[0][2] = .33;
  // gmmat[1][2] = .33;
  // gmmat[2][2] = .34;
  gmmat[0][1] = 1;
  gmmat[1][2] = 1;
  gmmat[2][0] = .33;
  gmmat[2][1] = .33;
  gmmat[2][2] = .34;
  return(1);
}

int GanitaMetricsMat::init(unsigned long nn, unsigned long mm)
{
  ncols = nn; nrows = mm;
  gmmat = new double *[nn] ;
  //memory allocated for elements of each column.
  
  for( unsigned long i = 0 ; i < nn; i++ )
    gmmat[i] = new double[mm]();

  return(1);
}

int GanitaMetricsMat::setUnit(unsigned long ii)
{
  unsigned long jj, kk;
  if(ii >= nrows){
    ii = ii % nrows;
  }
  for(jj=0; jj<ncols; jj++){
    for(kk=0; kk<nrows; kk++){
      gmmat[jj][kk] = 0;
    }
  }
  gmmat[0][ii] = 1;
  return(1);
}

