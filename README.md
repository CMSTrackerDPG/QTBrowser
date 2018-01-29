# How to create QTBrowser executable

## Prerequisits:
* Installed ROOT: Building ROOT from scratch is probably your safest bet. Enable openssl and TDavix in the cmake flags. (https://root.cern.ch/) 
* Installed Qt5


## How To
* git clone https://github.com/CMSTrackerDPG/QTBrowser.git
* cd QTbrowser
* qmake QTBrowser.pro
* make -j
* ./QTBrowser

## Remarks

You might need to adapt the path in the **QTBrowser.pro** file which is linking ROOT libraries.
Generally if you're seeing linking errors in your build, **QTBrowser.pro** is the place to look at. 

**!!!This needs to be fixed!!!**


# FOR DEVS -----------

# DQMFileDownloader

<code>dqmfiledownloader.h</code> is a QWidget that is embedded in the QTBrowser. Its purpose is to download files from the DQMgui.

It expects textfiles: 

* <code>data/online.txt</code>
* <code>data/offline.txt</code>
* <code>data/relval.txt</code>

Each of these textfiles should contain a path to a DQM file that can be downloaded from the DQMGui
e.g. **data/online.txt**

```
...
https://cmsweb.cern.ch/dqm/online/data/browse/Original/00030xxxx/0003060xx/DQM_V0001_TrackingHLTBeamspotStream_R000306029.root
https://cmsweb.cern.ch/dqm/online/data/browse/Original/00030xxxx/0003060xx/DQM_V0001_PixelPhase1_R000306029.root
https://cmsweb.cern.ch/dqm/online/data/browse/Original/00030xxxx/0003060xx/DQM_V0001_SiStrip_R000306029.root
...
```

This textfile is at startup loaded into a QList which displays the filename only.
One can download the files by clicking on "download". For this the <code>.pem</code> certificate needs to be loaded by the QTBrowser. (File>Settings)

It would therefore be great if the DQMGui could provide us with a list of files that are hosted on their site.

There is also a cpp-crawler that uses curl and is capable of collecting the data that we need from **online**, but NOT from **offline/relval**.
Check (needs certificate):

* https://cmsweb.cern.ch/dqm/online/data/browse
* https://cmsweb.cern.ch/dqm/offline/data/browse
* https://cmsweb.cern.ch/dqm/relval/data/browse

check out https://github.com/imKuehlschrank/CrawlerDqmGui: 

* Command line tool to update and crawl the **online** dqmgui
* Could be integrated into the QTBrowser if this is the path forward and we're not able to get the filelist from DQMGui.
* IMO this is not the way to go, and the DQMGui people should provide a list of files.


## Remarks
* The requests to download files are done through TDaviX.
* This means that ROOT needs to be built with **openssl** and **TDaviX**. This can be configured in the qmake file. Usually this should be the case, but sometimes root changes the default build configuration.


# Building for vocms061

On **vocms061** we dont have the luxury of Qt5, and we have to make do with Qt4.
In the qmake file(QTBrowser.pro) there is a build flag, **ON_VOCMS** that makes sure that Qt4 is being used, and that ROOT is linked from a recent **CMSSW** release.

There are some minor <code>ifdef's</code> in the code depending on the Qt version which only affect performance. Most notably <code>RemoteFilesModel::rowCount</code>.

# Run on vocms061

* cd /data/users/filic/
* cd CMSSW_9_4_0/
* cmsenv
* cd ../
* ./updateOnlineIndex.sh (**OPTIONAL**; uses CrawlerDqmGui)
* ./startQTBrowser.sh
