CREATE DATABASE shieldDB;

USE shieldDB;

CREATE TABLE PointsOfEntry(
	EntryName varchar(30), 
	PRIMARY KEY(EntryName),
	SystemName varchar(30),
	Locked int NOT NULL DEFAULT 1,
	Open int NOT NULL DEFAULT 0
	);

CREATE TABLE KnownPersons(
	Name varchar(30),
	PRIMARY KEY(Name),
	LastVisit DATETIME,
	Image varchar(100) NOT NULL,
	Permissions int NOT NULL DEFAULT 0
	);

CREATE TABLE AllVisitors(
	TimeVisited DATETIME NOT NULL,
	PRIMARY KEY(TimeVisited),
	Image varchar(200) NOT NULL,
	Known int NOT NULL DEFAULT 0,
	ACK int NOT NULL DEFAULT 0
	);

CREATE TABLE AlarmHistory(
	AlarmTime DATETIME, 
	PRIMARY KEY(AlarmTime),
	SystemName varchar(30) NOT NULL,
	Image varchar(100),
	ACK int NOT NULL DEFAULT 0,
	TriggeredBy varchar(30) NOT NULL
	);

CREATE TABLE System(
	Name varchar(30),
	PRIMARY KEY(Name),
	Armed int NOT NULL DEFAULT 0,
	AlarmActive int NOT NULL DEFAULT 0
	);

INSERT INTO System (Name,Armed,AlarmActive) VALUES ('Main',0,0);

CREATE TABLE Motion(
	Name varchar(30),
	PRIMARY KEY (Name),
	SystemName varchar(30) NOT NULL DEFAULT 'Main',
	MotionFound int DEFAULT 0 NOT NULL, 
	TriggerAlarm int DEFAULT 0 NOT NULL,
	TimeFound DATETIME
	);
