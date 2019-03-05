CREATE TABLE Locations 
(
	LocationId int not null identity primary key,
	Latitude nvarchar(50) not null,
	Longitude nvarchar(50) not null

)

CREATE TABLE DeviceTypes
(
	DeviceTypeId int not null identity primary key,
	DeviceType nvarchar(50) not null
)
GO

CREATE TABLE Students
(
	StudentId int not null identity primary key,
	StudentName nvarchar(50) not null
)
GO

CREATE TABLE Devices
(
	DeviceId int not null identity primary key,
	MacAdress nvarchar(50) not null,
	LocationId int foreign key references Locations(LocationId),
	DeviceTypeId int foreign key references DeviceTypes(DeviceTypeId),
	StudentId int foreign key references Students(StudentId)
)
GO

CREATE TABLE Messages
(
	MessageId int not null identity primary key,
	DeviceId int foreign key references Devices(DeviceId),
	Temperature decimal (4,1) not null,
	Humidity tinyint not null,
	Created datetime not null
)
GO