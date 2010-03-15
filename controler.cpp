 
 #include <iostream>
 #include <dirent.h>
 #include <sys/stat.h>
 #include <sys/types.h>
 #include <QFile>
 #include <QProgressDialog>
 #include <QFileInfo>
 #include <QTextStream>
 #include <QDir>
 #include "CProgresDialog.h"
 #include "controler.h"
 
 
 Controler::Controler(){
  devicesMutex = new QMutex();
 }
 
 int Controler::vetsi(QString  a, QString  b){
    if(a > b) return 0;
    else return 1;
 }
 
 QStringList Controler::cGetFiles(QDir path)
 {
    QDir directory(path);
    directory.setSorting(QDir::IgnoreCase);
    return directory.entryList();
 }
 

 QString Controler::cShowDiskUsage()
 {
	QString a(system("df -h"));
	return a;
 }
 
 bool Controler::cCopy(QString source,QString destination)
 {	
  int size = useDev->size();
  int sa=0,sb=0,da=0,db=0;
  std::cout << "Pred mutex lock" << std::endl;
  devicesMutex->lock();
  std::cout << "Po mutex lock" << std::endl;
  for(int i = 0; i < size; i++){
    if(source.startsWith(useDev->at(i).path) && useDev->at(i).path.size() > sa) {
      sa = useDev->at(i).path.size();
      sb = i;
    }
    if(destination.startsWith(useDev->at(i).path) && useDev->at(i).path.size() > da) {
      da = useDev->at(i).path.size();
      db = i;
    }
  }
  if( useDev->at(sb).use == 0 || useDev->at(db).use == 0){
    CProgresDialog *progrDialog = new CProgresDialog();
    progrDialog->setSource(source);
    progrDialog->setDestination(destination);
    progrDialog->setDevices((void *)useDev);
    progrDialog->setDevicesMutex((void *)devicesMutex);
    progrDialog->type = 1;
    progrDialog->show();
    progrDialog->activateWindow();
    useDev->at(sb).use = 1;
    useDev->at(db).use = 1;
    devicesMutex->unlock();
    return true;
  }else{
    devicesMutex->unlock();
    return false;
  }
 }
 bool Controler::cMove(QString source,QString destination)
 {
  int size = useDev->size();
  int sa=0,sb=0,da=0,db=0;
  for(int i = 0; i < size; i++){
    if(source.startsWith(useDev->at(i).path) && useDev->at(i).path.size() > sa) {
      sa = useDev->at(i).path.size();
      sb = i;
    }
    if(destination.startsWith(useDev->at(i).path) && useDev->at(i).path.size() > da) {
      da = useDev->at(i).path.size();
      db = i;
    }
  }
  if( useDev->at(sb).use == 0 || useDev->at(db).use == 0){
	 CProgresDialog *progrDialog = new CProgresDialog();
	 progrDialog->setSource(source);
	 progrDialog->setDestination(destination);
	 progrDialog->setDevices((void *)useDev);
	 progrDialog->setDevicesMutex((void *)devicesMutex);
	 progrDialog->type = 2;
	 progrDialog->show();
	 progrDialog->activateWindow();
	 useDev->at(sb).use = 1;
   useDev->at(db).use = 1;
   return true;
 }else{
  return false;
 }
 }
 void Controler::cDelete(QString source)
 {
	 CProgresDialog *progrDialog = new CProgresDialog();
	 progrDialog->setSource(source);
	 progrDialog->setDestination("");
	 progrDialog->type = 3;
	 progrDialog->show();
	 progrDialog->activateWindow();
// 	t_delete.setSource(source);
// 	t_delete.mutex.unlock(); 
 }

 int Controler::cMkDir(QString path, QString name){
	 QDir dir(path);
	 return dir.mkdir(name);
 }

 int Controler::cRename(QString source, QString destination){
	QFile file(source);
	return file.rename(destination);	
 } 

 QStringList Controler::cGetDevices(){
  QStringList devices;
  system("./script.sh");
  QFile file("devices.txt");
  useDev = new std::vector<struct device>();
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine();
      struct device tmp;
      tmp.path = line;
      tmp.use = 0;
      useDev->push_back(tmp);
      devices << line;
    }
  }
  return devices;
 }
 
 
 
