-- MySQL dump 10.13  Distrib 5.6.21, for osx10.8 (x86_64)
--
-- Host: localhost    Database: shieldDB
-- ------------------------------------------------------
-- Server version	5.6.21

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `AllVisitors`
--

DROP TABLE IF EXISTS `AllVisitors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `AllVisitors` (
  `TimeVisited` datetime NOT NULL,
  `Image` varchar(200) NOT NULL,
  `Known` int(11) NOT NULL DEFAULT '0',
  `ACK` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`TimeVisited`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `AllVisitors`
--

LOCK TABLES `AllVisitors` WRITE;
/*!40000 ALTER TABLE `AllVisitors` DISABLE KEYS */;
INSERT INTO `AllVisitors` VALUES ('2014-11-29 13:36:36','/capture/known/me.jpg',1,0),('2014-11-29 14:06:10','/capture/new/test.jpg',0,0),('2014-11-29 14:13:00','/capture/known/ryan.jpg',1,1);
/*!40000 ALTER TABLE `AllVisitors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `KnownPersons`
--

DROP TABLE IF EXISTS `KnownPersons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `KnownPersons` (
  `Name` varchar(30) NOT NULL DEFAULT '',
  `LastVisit` datetime DEFAULT NULL,
  `Image` varchar(100) NOT NULL,
  `Permissions` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `KnownPersons`
--

LOCK TABLES `KnownPersons` WRITE;
/*!40000 ALTER TABLE `KnownPersons` DISABLE KEYS */;
INSERT INTO `KnownPersons` VALUES ('Hitesh Patel','2014-11-29 13:36:36','/capture/known/me.jpg',1),('Ryan Stockhoff','2014-11-29 14:13:00','/capture/known/ryan.jpg',1);
/*!40000 ALTER TABLE `KnownPersons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Motion`
--

DROP TABLE IF EXISTS `Motion`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Motion` (
  `Name` varchar(30) NOT NULL DEFAULT '',
  `SystemName` varchar(30) NOT NULL DEFAULT 'Main',
  `MotionFound` int(11) NOT NULL DEFAULT '0',
  `TriggerAlarm` int(11) NOT NULL DEFAULT '0',
  `TimeFound` datetime DEFAULT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Motion`
--

LOCK TABLES `Motion` WRITE;
/*!40000 ALTER TABLE `Motion` DISABLE KEYS */;
INSERT INTO `Motion` VALUES ('Sens1','Main',0,0,'2014-11-29 11:21:31'),('Sens2','Main',1,0,'2014-11-29 11:09:13'),('Sens3','Main',1,1,'2014-11-29 11:29:20'),('Sens4','Main',1,1,'2014-11-29 11:21:25'),('Sens5','Main',1,1,'2014-11-29 14:35:22');
/*!40000 ALTER TABLE `Motion` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `System`
--

DROP TABLE IF EXISTS `System`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `System` (
  `Name` varchar(30) NOT NULL DEFAULT '',
  `Armed` int(11) NOT NULL DEFAULT '0',
  `AlarmActive` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `System`
--

LOCK TABLES `System` WRITE;
/*!40000 ALTER TABLE `System` DISABLE KEYS */;
INSERT INTO `System` VALUES ('Backup',0,0),('Main',0,1),('New House',0,0);
/*!40000 ALTER TABLE `System` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `alarmhistory`
--

DROP TABLE IF EXISTS `alarmhistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alarmhistory` (
  `AlarmTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `SystemName` varchar(30) NOT NULL,
  `Image` varchar(100) DEFAULT NULL,
  `ACK` int(11) NOT NULL DEFAULT '0',
  `TriggeredBy` varchar(30) NOT NULL,
  PRIMARY KEY (`AlarmTime`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `alarmhistory`
--

LOCK TABLES `alarmhistory` WRITE;
/*!40000 ALTER TABLE `alarmhistory` DISABLE KEYS */;
INSERT INTO `alarmhistory` VALUES ('2014-11-29 11:21:25','Main',NULL,1,'Sens4'),('2014-11-29 11:21:31','Main',NULL,1,'Sens1'),('2014-11-29 11:29:20','Main',NULL,1,'Sens3'),('2014-11-29 14:35:22','Main','/test/path/img.jpg',0,'Sens5');
/*!40000 ALTER TABLE `alarmhistory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pointsofentry`
--

DROP TABLE IF EXISTS `pointsofentry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pointsofentry` (
  `EntryName` varchar(30) NOT NULL DEFAULT '',
  `SystemName` varchar(30) DEFAULT NULL,
  `Locked` int(11) NOT NULL DEFAULT '1',
  `Open` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`EntryName`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pointsofentry`
--

LOCK TABLES `pointsofentry` WRITE;
/*!40000 ALTER TABLE `pointsofentry` DISABLE KEYS */;
INSERT INTO `pointsofentry` VALUES ('POE1','Main',1,0),('Window','Main',1,0);
/*!40000 ALTER TABLE `pointsofentry` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-12-01  9:49:00
