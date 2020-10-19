// ----------------------------------------------------------------------
// -----           EnsarBoxGenerator source file                    -----
// ----------------------------------------------------------------------

#include "EnsarBoxGenerator.h"

#include "FairPrimaryGenerator.h"

#include "TRandom.h"
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"

// ------------------------------------------------------------------------
EnsarBoxGenerator::EnsarBoxGenerator() :
  FairGenerator(),
  fPDGType(0),fMult(0),fPDGMass(0),fPtMin(0),fPtMax(0),
  fPhiMin(0),fPhiMax(0),fEtaMin(0),fEtaMax(0),fYMin(0),fYMax(0),
  fPMin(0),fPMax(0),fThetaMin(0),fThetaMax(0),fX(0),fY(0),fZ(0),
  fX1(0),fY1(0),fX2(0),fY2(0),
  fEtaRangeIsSet(0),fYRangeIsSet(0),fThetaRangeIsSet(0),
  fCosThetaIsSet(0),fPtRangeIsSet(0),fPRangeIsSet(0),
  fPointVtxIsSet(0),fBoxVtxIsSet(0),fDebug(0),
  fBeta(0),fRandomBeta(0),fBetaMax(0),fBetaMin(0)
{
  // Default constructor
}

// ------------------------------------------------------------------------
EnsarBoxGenerator::EnsarBoxGenerator(Int_t pdgid, Int_t mult) :
  fPDGType(pdgid),fMult(mult),fPDGMass(0),fPtMin(0),fPtMax(0),
  fPhiMin(0),fPhiMax(0),fEtaMin(0),fEtaMax(0),fYMin(0),fYMax(0),
  fPMin(0),fPMax(0),fThetaMin(0),fThetaMax(0),fX(0),fY(0),fZ(0),
  fX1(0),fY1(0),fX2(0),fY2(0),
  fEtaRangeIsSet(0), fYRangeIsSet(0),fThetaRangeIsSet(0),
  fCosThetaIsSet(0), fPtRangeIsSet(0), fPRangeIsSet(0),
  fPointVtxIsSet(0),fBoxVtxIsSet(0),fDebug(0),
  fBeta(0),fRandomBeta(0),fBetaMax(0),fBetaMin(0)
{
  // Constructor. Set default kinematics limits
  SetPhiRange  ();
}
// ------------------------------------------------------------------------
Bool_t  EnsarBoxGenerator::Init()
{
  // Initialize generator


  if (fPhiMax-fPhiMin>360)
    Fatal("Init()","EnsarBoxGenerator: phi range is too wide: %f<phi<%f",
          fPhiMin,fPhiMax);
  if (fPRangeIsSet && fPtRangeIsSet) {
    Fatal("Init()","EnsarBoxGenerator: Cannot set P and Pt ranges simultaneously");
  }
  if (fPRangeIsSet && fYRangeIsSet) {
    Fatal("Init()","EnsarBoxGenerator: Cannot set P and Y ranges simultaneously");
  }
  if ( (fThetaRangeIsSet && fYRangeIsSet) ||
       (fThetaRangeIsSet && fEtaRangeIsSet) ||
       (fYRangeIsSet     && fEtaRangeIsSet) ) {
    Fatal("Init()","EnsarBoxGenerator: Cannot set Y, Theta or Eta ranges simultaneously");
  }
  if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarBoxGenerator: Cannot set point and box vertices simultaneously");
  }

  // Check for particle type
  TDatabasePDG* pdgBase = TDatabasePDG::Instance();
  TParticlePDG* particle = pdgBase->GetParticle(fPDGType);

  if (! particle) {
    Fatal("EnsarBoxGenerator","PDG code %d not defined.",fPDGType);
  } else {
    fPDGMass = particle->Mass();
    // printf("particle->Mass() = %f \n", fPDGMass);
    return kTRUE;
  }
  return kTRUE;
}

// ------------------------------------------------------------------------
Bool_t EnsarBoxGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  // Generate one event: produce primary particles emitted from one vertex.
  // Primary particles are distributed uniformly along
  // those kinematics variables which were limitted by setters.
  // if SetCosTheta() function is used, the distribution will be uniform in
  // cos(theta)

  Double32_t pabs=0, phi, pt=0, theta=0, eta, y, mt, px, py, pz=0;

  // Generate particles
  for (Int_t k = 0; k < fMult; k++) {
    phi = gRandom->Uniform(fPhiMin,fPhiMax) * TMath::DegToRad();

    if      (fPRangeIsSet ) { pabs = gRandom->Uniform(fPMin,fPMax); }
    else if (fPtRangeIsSet) { pt   = gRandom->Uniform(fPtMin,fPtMax); }

    if      (fThetaRangeIsSet) {
      if (fCosThetaIsSet)
        theta = acos(gRandom->Uniform(cos(fThetaMin* TMath::DegToRad()),
                                      cos(fThetaMax* TMath::DegToRad())));
      else {
        theta = gRandom->Uniform(fThetaMin,fThetaMax) * TMath::DegToRad();
      }
    } else if (fEtaRangeIsSet) {
      eta   = gRandom->Uniform(fEtaMin,fEtaMax);
      theta = 2*TMath::ATan(TMath::Exp(-eta));
    } else if (fYRangeIsSet) {
      y     = gRandom->Uniform(fYMin,fYMax);
      mt = TMath::Sqrt(fPDGMass*fPDGMass + pt*pt);
      pz = mt * TMath::SinH(y);
    }

    if (fThetaRangeIsSet || fEtaRangeIsSet) {
      if      (fPRangeIsSet ) {
        pz = pabs*TMath::Cos(theta);
        pt = pabs*TMath::Sin(theta);
      } else if (fPtRangeIsSet) {
        pz = pt/TMath::Tan(theta);
      }
    }

    px = pt*TMath::Cos(phi);
    py = pt*TMath::Sin(phi);

    if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
    }

    if (fDebug)
      printf("BoxGen: kf=%d, p=(%.2f, %.2f, %.2f) GeV, x=(%.1f, %.1f, %.1f) cm\n",
             fPDGType, px, py, pz, fX, fY, fZ);

    Double32_t beta;
    if(fRandomBeta) {
      beta = gRandom->Uniform(fBetaMin,fBetaMax);
    } else {
      beta = fBeta;
    }
    Double32_t gamma = TMath::Sqrt(1-beta*beta);
    Double32_t momentum=TMath::Sqrt(px*px+py*py+pz*pz);
    pz = (pz + beta * momentum) / gamma;

    primGen->AddTrack(fPDGType, px, py, pz, fX, fY, fZ);
  }
  return kTRUE;

}
// ------------------------------------------------------------------------


ClassImp(EnsarBoxGenerator)
