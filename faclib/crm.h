/*
 *   FAC - Flexible Atomic Code
 *   Copyright (C) 2001-2015 Ming Feng Gu
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CRM_H_
#define _CRM_H_ 1

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "array.h"
#include "dbase.h"
#include "rates.h"
#include "nucleus.h"
#include "interpolation.h"
#include "coulomb.h"
#include "init.h"

#define RATES_BLOCK   256000
#define ION_BLOCK     128
#define LBLOCK_BLOCK  8192

#define MAXNREC 128
typedef struct _RECOMBINED_ {
  int bmin, bmax;
  int n, n_ext;
  int imin[MAXNREC];
  int imax[MAXNREC];
  int nrec[MAXNREC];
} RECOMBINED;

typedef struct _LBLOCK_ {
  int ib;
  int iion;
  int irec;
  int ionized;
  RECOMBINED *rec;
  int imin;
  int nlevels;
  double nb;
  double *n, *n0;
  double *r;
  double *total_rate;
  double *rc0, *rc1;
  int izr;
  NCOMPLEX ncomplex[MAXNCOMPLEX];
} LBLOCK;

typedef struct _BLK_RATE_ {
  LBLOCK *iblock;
  LBLOCK *fblock;
  ARRAY *rates;
} BLK_RATE;

typedef struct _ION_ {
  int iground; /* ionized ground state of this ion */
  int ground; /* ground state index of this ion */
  int nlevels;
  LBLOCK **iblock;
  int *ilev;
  int *j;
  short *p;
  short *vnl, *vni;
  short *sw;
  short *ibase;
  double *energy;
  ARRAY *ce_rates;
  ARRAY *tr_rates;
  ARRAY *tr_sdev;
  ARRAY *tr2_rates;
  ARRAY *ci_rates;
  ARRAY *rr_rates;
  ARRAY *ai_rates;
  ARRAY *cx_rates;
  ARRAY *recombined;
  int *icx[4];
  int nele;
  char *dbfiles[NDB1];
  double n, nt, n0;
  int KLN_min, KLN_max;
  int KLN_bmin, KLN_bmax;
  int KLN_amin, KLN_amax;
  double *KLN_ai;
  int *KLN_nai;
  double ace, atr, aci, arr, aai, acx;
} ION;

typedef struct _IONIZED_ {
  int nele;
  char symbol[4];
  double atom;
  char *dbfiles[NDB1];
  int nionized;
  int *ionized_map[2];
  int imin[2], imax[2];
  double *energy;
  double n, nt, n0;
  double ace, atr, aci, arr, aai, acx;
} IONIZED;

typedef struct _RATE_ {
  int i, f;
  double dir;
  double inv;
} RATE;

typedef struct _LINETYPE_ {
  int type;
  int nele;
} LINETYPE;

typedef struct _LINEREC_ {
  int z;
  int nele;
  int type;
  int nmin, nmax;
  int ia, nr;
  double *e, *s, *w0, *w, *n0, *n1, *k;
  double ae, aw, nt, ni;
} LINEREC;

typedef struct _INTERPSP_ {
  int nd, ds;
  double *xd;
  int nt, ts;
  double *xt;
  double smin;
  double tsize, maxmem;
  char fn[1024];
  LINEREC **r;
} INTERPSP;

int SetNumSingleBlocks(int n);
int SetEleDensity(double ele);
int SetCxtDensity(double cxt);
int SetPhoDensity(double pho);
int SetCascade(int c, double a);
int SetIteration(double acc, double s, int max);
int InitCRM(void);
int ReinitCRM(int m);
int AddIon(int nele, double n, char *pref);
int IonIndex(ION *ion, int i, int k);
int IonizedIndex(int i, int m);
void GetRecombined(int *b, int *nrec, char *name);
int CopyNComplex(NCOMPLEX *dest, NCOMPLEX *src);
int CompareNComplex(NCOMPLEX *c1, NCOMPLEX *c2);
int StrNComplex(char *s, NCOMPLEX *c);
int TransitionType(NCOMPLEX *ic, NCOMPLEX *fc);
void ExtrapolateEN(int i, ION *ion);
int FindFinalTR(ION *ion, int f, int n1, int n0);
void ExtrapolateTR(ION *ion, int inv, int **irb);
void ExtrapolateRR(ION *ion, int inv, int **irb);
void ExtrapolateAI(ION *ion, int inv, int **irb);
int SetBlocks(double ni, char *ifn);
void SetRateMultiplier(int nele, int t, double a);
int SetAbund(int nele, double abund);
int InitBlocks(void);
int AddRate(ION *ion, ARRAY *rts, RATE *r, int m, int **irb);
int SetCERates(int inv);
int SetCXRates(int m, char *tgt);
int SetTRRates(int inv);
int SetCIRates(int inv);
int SetRRRates(int inv);
int SetAIRates(int inv);
int SetAIRatesInner(char *fn);
int RateTable(char *fn, int nc, char *sc[], int md);
int BlockMatrix(void);
int BlockPopulation(int n);
double BlockRelaxation(int iter);
int LevelPopulation(void);
int Cascade(void);
int SpecTable(char *fn, int rrc, double smin);
int SelectLines(char *ifn, char *ofn, int nele, int type, 
		double emin, double emax, double fmin);
int PlotSpec(char *ifn, char *ofn, int nele, int type,
	     double emin, double emax, double de, double smin);
int DRBranch(void);
int DRStrength(char *fn, int nele, int mode, int ilev0);
int DumpRates(char *fn, int k, int m, int imax, int a);
int ModifyRates(char *fn);
int SetInnerAuger(int i);
int SetExtrapolate(int e);
void TabNLTE(char *fn1, char *fn2, char *fn3, char *fn,
	     double xmin, double xmax, double dx);
int SetEMinAI(double e);
int DRSuppression(char *fn, double z, int nmax);
int RydBranch(char *fn, char *ofn, int n0, int n1);
int NormalizeMode(int m);
void FixNorm(int m);
void SetOptionCRM(char *s, char *sp, int ip, double dp);
void PrepInterpSpec(int nd, double d0, double d1, int ds,
		    int nt, double t0, double t1, int ts,
		    double smin, double maxmem, char *fn);
void InterpSpec(int nele, int type, int nmin, int nmax, double c,
		double d, double t, double s, int n, double *x, double *y);
void InterpSpecWF(char *fn, int nele, int type, int nmin, int nmax,
		  double c, double d, double t, double s,
		  int n, double emin, double emax);
void ConvLineRec(int n, double *x, double *y,
		 double mt, double d0, double t0,
		 double s, double dw, double c,
		 double e, double w, LINEREC *r);
void LoadLineRec(int id0, int it0, int nele,
		 int type, int nmin, int nmax);
void PrepStarkQC(double mt, double d0, double t0,
		 double *wd, double *wdi, double *wir,
		 int zt, int ne0, int ne1, double *wrf, double *wid);
double CalcStarkQC(double w0, double wd, double *wdi,
		   double *wir, double *wrf, double *wid,
		   int nele, int type);
double LimitImpactWidth(double zd, double de);
void SetStarkZMP(int np, double *wzm);
double MicroFieldDist(double x, double g, double s);
double MicroFieldMode(double g, double s);
double QSReduction(double g, double s);
double DebyeLength(double d, double t);
void  ScaledSG(double s, double g, double zr, double *sn, double *gn);
void UnscaledSG(double zp, double te, double de, double *s0, double *g0);
void SortBranches(ARRAY *rts, IDXARY *idx, IDXARY *fdx,
		  int ig, int *nr, RATE **rs);
double DRSupFactor(double z0, double d0, double t0);
void RateCoefficients(char *ofn, int k0, int k1, int nexc, int ncap,
		      int nt, double t0, double t1,
		      int nd, double d0, double d1, int md);
ARRAY* _GetIons();  // Add an access to ions for testing purpose
#endif

