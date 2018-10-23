// -*- C++ -*-
//
// Package:    Image/ImageAnalyzer
// Class:      ImageAnalyzer
//
/**\class ImageAnalyzer ImageAnalyzer.cc Image/ImageAnalyzer/plugins/ImageAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
//
//
// Code examples used to write this code
// https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/DataFormats/TrackReco/interface/TrackBase.h
// http://cmslxr.fnal.gov/source/Fireworks/Tracks/plugins/FWTracksRecHitsProxyBuilder.cc
// http://cmslxr.fnal.gov/source/Fireworks/Tracks/src/TrackUtils.cc#0422
// http://cmslxr.fnal.gov/source/Fireworks/Geometry/src/FWRecoGeometryESProducer.cc#0075
// http://cmslxr.fnal.gov/source/Fireworks/Geometry/interface/FWRecoGeometryESProducer.h#0059


// system include files
#include <memory>
#include <cmath>
#include <stdlib.h>
#include <time.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// track includes
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// PFCluster
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"

// Muons
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "Geometry/MuonNumbering/interface/MuonSubDetector.h"

// pngwriter
#include "Demo/ImageAnalyzer/interface/pngwriter.h"


// fireworks and geometry includes
#include "TEveGeoShape.h"
#include "TEvePointSet.h"

#include "Fireworks/Core/interface/FWSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWGeometry.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Tracks/interface/TrackUtils.h"
#include "Fireworks/Core/interface/fwLog.h"

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit1D.h"

#include <iostream>
using namespace std;

// helper function to get scaled energy/color
double eColor(double e, double maxE);
double eColor(double e, double maxE) {
    if (e/maxE > 1) {
        return 1;
    }
    return e/maxE;
}

// helper function to add time stamp to image file name
// makes file names unique as run, event, & lumi triplets eventually overlap
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);

    return buf;
}


// local function to get Pixel Detector Hits, see
// http://cmslxr.fnal.gov/source/Fireworks/Tracks/src/TrackUtils.cc#0611
void
pixelHits( std::vector<TVector3> &pixelPoints, FWGeometry *geom, const reco::Track &t );
void
pixelHits( std::vector<TVector3> &pixelPoints, FWGeometry *geom, const reco::Track &t )
{
   for( trackingRecHit_iterator it = t.recHitsBegin(), itEnd = t.recHitsEnd(); it != itEnd; ++it )
   {
      const TrackingRecHit* rh = &(**it);
      // -- get position of center of wafer, assuming (0,0,0) is the center
      DetId id = (*it)->geographicalId();
      if( ! geom->contains( id ))
      {
         fwLog( fwlog::kError )
            << "failed to get geometry of Tracker Det with raw id: "
            << id.rawId() << std::endl;

        continue;
      }

      // -- in which detector are we?
      unsigned int subdet = (unsigned int)id.subdetId();

      if(( subdet == PixelSubdetector::PixelBarrel ) || ( subdet == PixelSubdetector::PixelEndcap ))
      {
         if( const SiPixelRecHit* pixel = dynamic_cast<const SiPixelRecHit*>( rh ))
         {
            const SiPixelCluster& c = *( pixel->cluster());
            fireworks::pushPixelCluster( pixelPoints, *geom, id, c, geom->getParameters( id ));
         }
      }
   }
}

const SiStripCluster* extractClusterFromTrackingRecHit( const TrackingRecHit* rechit );
const SiStripCluster* extractClusterFromTrackingRecHit( const TrackingRecHit* rechit )
{
   const SiStripCluster* cluster = 0;

   if( const SiStripRecHit2D* hit2D = dynamic_cast<const SiStripRecHit2D*>( rechit ))
   {
	 cluster = hit2D->cluster().get();
   }
   if( cluster == 0 )
   {
     if( const SiStripRecHit1D* hit1D = dynamic_cast<const SiStripRecHit1D*>( rechit ))
     {
	   cluster = hit1D->cluster().get();
     }
   }
   return cluster;
}
void
SiStripClusters( std::vector<TVector3> &points, FWGeometry *geom, const reco::Track &t );
void
SiStripClusters( std::vector<TVector3> &points, FWGeometry *geom, const reco::Track &t )
{
   bool addNearbyClusters = true;
   const edmNew::DetSetVector<SiStripCluster> * allClusters = 0;
   if( addNearbyClusters )
   {
      for( trackingRecHit_iterator it = t.recHitsBegin(), itEnd = t.recHitsEnd(); it != itEnd; ++it )
      {
         if( typeid( **it ) == typeid( SiStripRecHit2D ))
         {
            const SiStripRecHit2D &hit = static_cast<const SiStripRecHit2D &>( **it );
            if( hit.cluster().isNonnull() && hit.cluster().isAvailable()) {
               edm::Handle<edmNew::DetSetVector<SiStripCluster> > allClustersHandle;
               allClusters = allClustersHandle.product();
               break;
            }
         }
         else if( typeid( **it ) == typeid( SiStripRecHit1D ))
         {
            const SiStripRecHit1D &hit = static_cast<const SiStripRecHit1D &>( **it );
            if( hit.cluster().isNonnull() && hit.cluster().isAvailable())
            {
               edm::Handle<edmNew::DetSetVector<SiStripCluster> > allClustersHandle;
               allClusters = allClustersHandle.product();
               break;
            }
         }
      }
   }

   for( trackingRecHit_iterator it = t.recHitsBegin(), itEnd = t.recHitsEnd(); it != itEnd; ++it )
   {
      unsigned int rawid = (*it)->geographicalId();
      if( ! geom->contains( rawid ))
      {
         fwLog( fwlog::kError )
           << "failed to get geometry of SiStripCluster with detid: "
           << rawid << std::endl;

         continue;
      }

      const float* pars = geom->getParameters( rawid );

      // -- get phi from SiStripHit
      auto rechitRef = *it;
      const TrackingRecHit *rechit = &( *rechitRef );
      const SiStripCluster *cluster = extractClusterFromTrackingRecHit( rechit );

      if( cluster )
      {
         if( allClusters != 0 )
         {
            const edmNew::DetSet<SiStripCluster> & clustersOnThisDet = (*allClusters)[rechit->geographicalId().rawId()];

            for( edmNew::DetSet<SiStripCluster>::const_iterator itc = clustersOnThisDet.begin(), edc = clustersOnThisDet.end(); itc != edc; ++itc )
            {

               short firststrip = itc->firstStrip();

               float localTop[3] = { 0.0, 0.0, 0.0 };
               float localBottom[3] = { 0.0, 0.0, 0.0 };

               fireworks::localSiStrip( firststrip, localTop, localBottom, pars, rawid );

               float globalTop[3];
               float globalBottom[3];
               geom->localToGlobal( rawid, localTop, globalTop, localBottom, globalBottom );

               TVector3 pt( globalTop[0], globalTop[1], globalTop[2] );
               points.push_back( pt );
               TVector3 pb( globalBottom[0], globalBottom[1], globalBottom[2] );
               points.push_back( pb );

            }
         }
         else
         {
            short firststrip = cluster->firstStrip();

            float localTop[3] = { 0.0, 0.0, 0.0 };
            float localBottom[3] = { 0.0, 0.0, 0.0 };

            fireworks::localSiStrip( firststrip, localTop, localBottom, pars, rawid );

            float globalTop[3];
            float globalBottom[3];
            geom->localToGlobal( rawid, localTop, globalTop, localBottom, globalBottom );

               TVector3 pt( globalTop[0], globalTop[1], globalTop[2] );
               points.push_back( pt );
               TVector3 pb( globalBottom[0], globalBottom[1], globalBottom[2] );
               points.push_back( pb );


         }
      }
      else if( !rechit->isValid() && ( rawid != 0 )) // lost hit
      {
         if( allClusters != 0 )
         {
            edmNew::DetSetVector<SiStripCluster>::const_iterator itds = allClusters->find( rawid );
            if( itds != allClusters->end())
            {
               const edmNew::DetSet<SiStripCluster> & clustersOnThisDet = *itds;
               for( edmNew::DetSet<SiStripCluster>::const_iterator itc = clustersOnThisDet.begin(), edc = clustersOnThisDet.end(); itc != edc; ++itc )
               {
                  short firststrip = itc->firstStrip();

                  float localTop[3] = { 0.0, 0.0, 0.0 };
                  float localBottom[3] = { 0.0, 0.0, 0.0 };

                  fireworks::localSiStrip( firststrip, localTop, localBottom, pars, rawid );

                  float globalTop[3];
                  float globalBottom[3];
                  geom->localToGlobal( rawid, localTop, globalTop, localBottom, globalBottom );

                   TVector3 pt( globalTop[0], globalTop[1], globalTop[2] );
                   points.push_back( pt );
                   TVector3 pb( globalBottom[0], globalBottom[1], globalBottom[2] );
                   points.push_back( pb );

               }
            }
         }
      }
      else
      {
         fwLog( fwlog::kDebug )
            << "*ANOTHER* option possible: valid=" << rechit->isValid()
            << ", rawid=" << rawid << std::endl;
      }
   }
}

double signLog1p(double v);
double signLog1p(double v)
{
    int s = 1;
    if (v < 0) s =-1;
//    cout << "v=" << v << " s=" << s << " abs(v)=" << abs(v) << " 1+=" << abs(v)+1 << " log=" << log(abs(v)+1) << endl;
    return s*log(abs(v)+1);
}

// makePNG1P transforms given hits into signLog1p values and put them in an image
void
makePNG1P( pngwriter &png, int width, int height, const std::string &fname, std::vector<TVector3> &points, double red, double green, double blue);
void
makePNG1P( pngwriter &png, int width, int height, int detX, int detY, int detZ, const std::string &fname, std::vector<TVector3> &points, double red, double green, double blue)
{
    int hdim = height/3;
    double bmaxx = signLog1p(detX); // detector X dimension we'll use in image
    double bminx =-signLog1p(detX);
    double bmaxy = signLog1p(detY); // detector Y dimension we'll use in image
    double bminy =-signLog1p(detY);
    double bmaxz = signLog1p(detZ); // detector Z dimension we'll use in image
    double bminz =-signLog1p(detZ);
    // picture boundaries
    double maxx = hdim; // e.g. 100
    double maxy = hdim; // e.g. 100
    double maxz = width; // e.g. 300
    for( auto it = points.begin(), itEnd = points.end(); it != itEnd; ++it) {
        // normalize to detector boundaries: (v-min_v)/(max_v-min_v)
//        auto z = signLog1p(it->z())/signLog1p(detZ);
//        auto x = signLog1p(it->x())/signLog1p(detX);
//        auto y = signLog1p(it->y())/signLog1p(detY);
        auto z = (signLog1p(it->z())-bminz)/(bmaxz-bminz); // max_v=maxz, min_v=-maxz
        auto x = (signLog1p(it->x())-bminx)/(bmaxx-bminx);
        auto y = (signLog1p(it->y())-bminy)/(bmaxy-bminy);
        // normalize to picture boundaries
        z = z*maxz;
        x = x*maxx;
        y = y*maxy;
        // xz projection (z along width)
        if(abs(z) <= width && z >= 0 && x <= hdim && x >= 0) {
            png.plot((int)z, (int)x, red, green, blue);
        }
        // xy projection (x along width)
        x = x+maxx; // center it on a plot
        if(x <= width && x >= 0 && y <= hdim && y >= 0) {
            png.plot((int)x, (int)(hdim+y), red, green, blue);
        }
        // yz projection (z along width)
        if(z <= width && z >= 0 && y <= hdim && y >= 0) {
            png.plot((int)z, (int)(2*hdim+y), red, green, blue);
        }
    }
}

// Example how to make a PNG file
void
makePNG( pngwriter &png, int width, int height, const std::string &fname, std::vector<TVector3> &points, double red, double green, double blue);
void
makePNG( pngwriter &png, int width, int height, int detX, int detY, int detZ, const std::string &fname, std::vector<TVector3> &points, double red, double green, double blue)
{
    int hdim = height/3;
    // detector boundaries
    double bmaxx = detX; // detector X dimension we'll use in image
    double bminx =-detX;
    double bmaxy = detY; // detector Y dimension we'll use in image
    double bminy =-detY;
    double bmaxz = detZ; // detector Z dimension we'll use in image
    double bminz =-detZ;
    // picture boundaries
    double maxx = hdim; // e.g. 100
    double maxy = hdim; // e.g. 100
    double maxz = width; // e.g. 300
    for( auto it = points.begin(), itEnd = points.end(); it != itEnd; ++it) {
        // normalize to detector boundaries: (v-min_v)/(max_v-min_v)
        auto z = (it->z()-bminz)/(bmaxz-bminz); // max_v=maxz, min_v=-maxz
        auto x = (it->x()-bminx)/(bmaxx-bminx);
        auto y = (it->y()-bminy)/(bmaxy-bminy);
        // normalize to picture boundaries
        z = z*maxz;
        x = x*maxx;
        y = y*maxy;
        // xz projection (z along width)
        if(abs(z) <= width && z >= 0 && x <= hdim && x >= 0) {
            png.plot((int)z, (int)x, red, green, blue);
        }
        // xy projection (x along width)
//        if(x <= width && x >= 0 && y <= hdim && y >= 0) {
//            png.plot((int)x, (int)(hdim+y), red, green, blue);
//        }
        x = x+maxx; // center it on a plot
        if(x <= width && x >= 0 && y <= hdim && y >= 0) {
            png.plot((int)x, (int)(hdim+y), red, green, blue);
        }
        // yz projection (z along width)
        if(z <= width && z >= 0 && y <= hdim && y >= 0) {
            png.plot((int)z, (int)(2*hdim+y), red, green, blue);
        }
    }
}

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class ImageAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit ImageAnalyzer(const edm::ParameterSet&);
      ~ImageAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      // FWGeometry
      FWGeometry *geom;

      // output directory where we store results (png's and numpy files)
      std::string outputDir;

      // png dimensions
      int png_width;
      int png_height;
      int detX;
      int detY;
      int detZ;
      int ecalMaxEnergy;
      int hcalMaxEnergy;
      int psMaxEnergy;
      int verbose;
      int useLog;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ImageAnalyzer::ImageAnalyzer(const edm::ParameterSet& iConfig)

{
   // parse configuration parameter set
   outputDir = iConfig.retrieveUntracked("outputDir")->getString();
   png_width = iConfig.retrieveUntracked("pngWidth")->getInt32();
   png_height = iConfig.retrieveUntracked("pngHeight")->getInt32();
   detX = iConfig.retrieveUntracked("detX")->getInt32();
   detY = iConfig.retrieveUntracked("detY")->getInt32();
   detZ = iConfig.retrieveUntracked("detZ")->getInt32();
   ecalMaxEnergy = iConfig.retrieveUntracked("ecalMaxEnergy")->getInt32();
   hcalMaxEnergy = iConfig.retrieveUntracked("hcalMaxEnergy")->getInt32();
   psMaxEnergy = iConfig.retrieveUntracked("psMaxEnergy")->getInt32();
   verbose = iConfig.retrieveUntracked("verbose")->getInt32();
   useLog = iConfig.retrieveUntracked("useLog")->getInt32();
   // load geometry
   geom = new FWGeometry();
   auto geomFile = iConfig.retrieveUntracked("geomFile")->getString();
   if (verbose > 0) {
       cout << "Read geometry from " << geomFile << endl;
   }
   geom->loadMap(geomFile.c_str());
   // a module register what data it will request from the Event, Chris' suggestion
   consumes<reco::TrackCollection>(edm::InputTag("generalTracks"));
   consumes<reco::PFClusterCollection>(edm::InputTag("particleFlowClusterECAL"));
   consumes<reco::PFClusterCollection>(edm::InputTag("particleFlowClusterHCAL"));
   consumes<reco::PFClusterCollection>(edm::InputTag("particleFlowClusterPS"));
   consumes<reco::MuonCollection>(edm::InputTag("muons"));
}


ImageAnalyzer::~ImageAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   geom->clear();
   delete geom;
}


//
// member functions
//

// ------------ method called for each event  ------------
void
ImageAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


   Handle<reco::TrackCollection> tracks;
   iEvent.getByLabel("generalTracks", tracks);
   LogInfo("Image") << "number of tracks "<<tracks->size();

   // extract Muons
   Handle<reco::MuonCollection> muons;
   iEvent.getByLabel("muons", muons);
   LogInfo("Image") << "number of muons " << muons->size();

   // extract ECAL clusters
   Handle<reco::PFClusterCollection> ecalClusters;
   iEvent.getByLabel("particleFlowClusterECAL", ecalClusters);
   LogInfo("Image") << "number of ecal clusters " << ecalClusters->size();

   // extract HCAL clusters
   Handle<reco::PFClusterCollection> hcalClusters;
   iEvent.getByLabel("particleFlowClusterHCAL", hcalClusters);
   LogInfo("Image") << "number of HCAL clusters " << hcalClusters->size();

   // extract PS clusters
   Handle<reco::PFClusterCollection> psClusters;
   iEvent.getByLabel("particleFlowClusterPS", psClusters);
   LogInfo("Image") << "number of ps clusters " << psClusters->size();

   // get event id
   auto eid = iEvent.id();

   // create PNG image of the event, we'll use in a single plot
   // 3 projections (xz, xy, yz) therefore we stack them as height*3
   std::string fname = outputDir+"/run"+std::to_string(eid.run())+"_evt"+std::to_string(eid.event())+"_lumi"+std::to_string(eid.luminosityBlock())+"_"+currentDateTime()+".png";
   auto height = png_height*3;
   auto width = png_width;
   pngwriter png( width, height, 0, fname.c_str());
   double red, green, blue;

   // loop over ECAL clusters
   int eidx = 0;
   for (auto cls = ecalClusters->cbegin(); cls != ecalClusters->end(); ++cls, ++eidx) {
       auto pos = cls->position();
       auto e = cls->energy();
       if (verbose > 1) {
           cout << "ECAL cls e=" << e << " x=" << pos.X() << " y=" << pos.Y() << " z=" << pos.Z() << endl;
       }
       red = eColor(e, (float)ecalMaxEnergy);
       green = eColor(e, (float)ecalMaxEnergy);
       blue = 1.0;
       std::vector<TVector3> pxpoints;
       TVector3 point(pos.X(), pos.Y(), pos.Z());
       pxpoints.push_back(point);
       if (useLog > 0) {
           makePNG1P(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
       } else {
           makePNG(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
       }
   }

   // loop over HCAL clusters
   int hidx = 0;
   for (auto cls = hcalClusters->cbegin(); cls != hcalClusters->end(); ++cls, ++hidx) {
       auto pos = cls->position();
       auto e = cls->energy();
       if (verbose > 1) {
           cout << "hcal cls e=" << e << " x=" << pos.X() << " y=" << pos.Y() << " z=" << pos.Z() << endl;
       }
       red = eColor(e, (float)hcalMaxEnergy);
       green = 1.0;
       blue = eColor(e, (float)hcalMaxEnergy);
       std::vector<TVector3> pxpoints;
       TVector3 point(pos.X(), pos.Y(), pos.Z());
       pxpoints.push_back(point);
       if (useLog > 0) {
           makePNG1P(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
       } else {
           makePNG(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
       }
   }

   // loop over PS clusters
   int pidx = 0;
   for (auto cls = psClusters->cbegin(); cls != psClusters->end(); ++cls, ++pidx) {
       auto pos = cls->position();
       auto e = cls->energy();
       if (verbose > 1) {
           cout << "PS cls e=" << e << " x=" << pos.X() << " y=" << pos.Y() << " z=" << pos.Z() << endl;
       }
       red = 1.0;
       green = eColor(e, (float)psMaxEnergy);
       blue = eColor(e, (float)psMaxEnergy);
       std::vector<TVector3> pxpoints;
       TVector3 point(pos.X(), pos.Y(), pos.Z());
       pxpoints.push_back(point);
       if (useLog > 0) {
           makePNG1P(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
       } else {
           makePNG(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
       }
   }

   // loop over tracks and get their hits
   for (auto muon = muons->cbegin();  muon != muons->end();  ++muon) {
       auto itrack = muon->innerTrack();
       if (itrack.isNonnull()) {
           auto pos = itrack->outerPosition();
           if (verbose > 1) {
               cout << "itrack outer muon x " << pos.X() << " y " << pos.Y() << " z " << pos.Z() << endl;
           }
       }
       auto otrack = muon->outerTrack();
       if (otrack.isNonnull()) {
           auto pos = otrack->outerPosition();
           if (verbose > 1) {
               cout << "otrack outer muon x " << pos.X() << " y " << pos.Y() << " z " << pos.Z() << endl;
           }
       }
       auto ttrack = muon->tpfmsTrack();
       if (ttrack.isNonnull()) {
           auto pos = ttrack->outerPosition();
           if (verbose > 1) {
               cout << "ttrack outer muon x " << pos.X() << " y " << pos.Y() << " z " << pos.Z() << endl;
           }
       }
       auto ptrack = muon->pickyTrack();
       if (ptrack.isNonnull()) {
           auto pos = ptrack->outerPosition();
           if (verbose > 1) {
               cout << "ptrack outer muon x " << pos.X() << " y " << pos.Y() << " z " << pos.Z() << endl;
           }
       }
       auto dtrack = muon->dytTrack();
       if (dtrack.isNonnull()) {
           auto pos = dtrack->outerPosition();
           if (verbose > 1) {
               cout << "dtrack outer muon x " << pos.X() << " y " << pos.Y() << " z " << pos.Z() << endl;
           }
       }
       auto track = muon->globalTrack();
       if (track.isNonnull()) {
           auto pos = track->outerPosition();
           if (verbose > 1) {
               cout << "gtrack outer muon x " << pos.X() << " y " << pos.Y() << " z " << pos.Z() << endl;
           }
           red = 1.0;
           green = 1.0;
           blue = 0.0;
           std::vector<TVector3> pxpoints;
           // since muon chambers are far away we use picture width as Z
           // position for muon hit instead of pos.Z()
//           TVector3 point(pos.X(), pos.Y(), detZ-1);
           TVector3 point(pos.X(), pos.Y(), pos.Z());
           pxpoints.push_back(point);
           if (useLog > 0) {
               makePNG1P(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
           } else {
               makePNG(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
           }
       }
   }

   // loop over tracks and get their hits
   int tidx = 0;
   std::vector<TVector3> hits;
   for (auto track = tracks->cbegin();  track != tracks->end();  ++track, ++tidx) {
       // extract Pixel hits
       std::vector<TVector3> pxpoints;
       pixelHits( pxpoints, geom, *track );
       if (verbose > 1) {
           cout << "pixel hits" << endl;
       }
       for( auto it = pxpoints.begin(), itEnd = pxpoints.end(); it != itEnd; ++it) {
           if (verbose > 1) {
               cout << it->x() << "," << it->y() << "," << it->z() << endl;
//               cout << "signLog " << signLog1p(it->x()) << "," << signLog1p(it->y()) << "," << signLog1p(it->z()) << endl;
           }
       }
       red = 1.0;
       green = 1.0;
       blue = 1.0;
       if (useLog > 0) {
           makePNG1P(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
       } else {
           makePNG(png, width, height, detX, detY, detZ, fname, pxpoints, red, green, blue);
       }

       // extract SiStripClusters
       std::vector<TVector3> sipoints;
       SiStripClusters(sipoints, geom, *track);
       if (verbose > 1) {
           cout << "SiStrip clusters" << endl;
       }
       for( auto it = sipoints.begin(), itEnd = sipoints.end(); it != itEnd; ++it) {
           if (verbose > 1) {
               cout << it->x() << "," << it->y() << "," << it->z() << endl;
//               cout << "signLog " << signLog1p(it->x()) << "," << signLog1p(it->y()) << "," << signLog1p(it->z()) << endl;
           }
       }
       red = 0.0;
       green = 1.0;
       blue = 1.0;
       if (useLog > 0) {
           makePNG1P(png, width, height, detX, detY, detZ, fname, sipoints, red, green, blue);
       } else {
           makePNG(png, width, height, detX, detY, detZ, fname, sipoints, red, green, blue);
       }
   }
   // close PNG image
   png.close();
}


// ------------ method called once each job just before starting event loop  ------------
void
ImageAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
ImageAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ImageAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ImageAnalyzer);
