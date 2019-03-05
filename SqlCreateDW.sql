SET DATEFIRST 1;
SET DATEFORMAT ymd;
SET LANGUAGE Swedish;

CREATE TABLE DimDate
(
	[DateTimeKey]	DATETIME PRIMARY KEY, 
	[Date]			AS CONVERT(DATE, [DateTimeKey]), 
	[Year]			AS DATEPART(YEAR, [DateTimeKey]),  
	[Quarter]		AS DATEPART(QUARTER, [DateTimeKey]),
	[QuarterName]	AS CONVERT(CHAR(2), CASE DATEPART(QUARTER, [DateTimeKey]) WHEN 1 THEN 'Q1' WHEN 2 THEN 'Q2' WHEN 3 THEN 'Q3' WHEN 4 THEN 'Q4' END),  
	[Month]			AS DATEPART(MONTH, [DateTimeKey]),  
	[MonthName]		AS DATENAME(MONTH, [DateTimeKey]),
	[WeekdayOfMonth]AS DATEPART(DAY, [DateTimeKey]),
	[WeekdayName]	AS DATENAME(WEEKDAY, [DateTimeKey]),
	[DayOfWeek]		AS DATEPART(WEEKDAY, [DateTimeKey]),
	[DayOfYear]		AS DATEPART(DAYOFYEAR, [DateTimeKey]),
	[Hour]			AS DATEPART(HOUR, [DateTimeKey]),
	[Minute]		AS DATEPART(MINUTE, [DateTimeKey]),
	[Second]		AS DATEPART(SECOND,	[DateTimeKey]),
	[Milisecond]	AS DATEPART(MILLISECOND, [DateTimeKey])
)

CREATE TABLE DimLocations
(
	[LocationId]	INT PRIMARY KEY,
	[Longitude]		NVARCHAR(50) NOT NULL,
	[Latitude]		NVARCHAR(50) NOT NULL
)

CREATE TABLE DimDeviceTypes
(
	[DeviceTypeId]	INT PRIMARY KEY,
	[DeviceType]	NVARCHAR(50) NOT NULL
)

CREATE TABLE DimStudents
(
	[StudentId]	INT PRIMARY KEY,
	[StudentName]	NVARCHAR(50) NOT NULL
)

GO

CREATE TABLE DimDevices
(
	[DeviceId]		INT	PRIMARY KEY,
	[MacAdress]		NVARCHAR(50) NOT NULL,
	[LocationId]	INT FOREIGN KEY REFERENCES DimLocations(LocationId),
	[DeviceTypeId]	INT FOREIGN KEY REFERENCES DimDeviceTypes(DeviceTypeId),
	[StudentId]		INT FOREIGN KEY REFERENCES DimStudents(StudentId)
)
GO

CREATE TABLE FactMessages
(
	[MessageId]		INT IDENTITY PRIMARY KEY,
	[Temperature]	FLOAT NOT NULL,
	[Humidity]		TINYINT NOT NULL,
	[Created]		DATETIME FOREIGN KEY REFERENCES DimDate(DateTimeKey),
	[DeviceId]		INT FOREIGN KEY REFERENCES DimDevices(DeviceId),
	[LocationId]	INT FOREIGN KEY REFERENCES DimLocations(LocationId),
	[DeviceTypeId]	INT FOREIGN KEY REFERENCES DimDeviceTypes(DeviceTypeId),
	[StudentId]		INT FOREIGN KEY REFERENCES DimStudents(StudentId)
)
GO
