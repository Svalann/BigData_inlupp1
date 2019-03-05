
IF NOT EXISTS (SELECT 1 FROM [Locations] WHERE [Longitude] = @devicelongitude AND [Latitude] = @devicelatitude) INSERT INTO [Locations] ([Longitude], [Latitude]) output INSERTED.LocationId VALUES(@devicelongitude, @devicelatitude) ELSE SELECT LocationId FROM [Locations] WHERE [Longitude] = @devicelongitude AND [Latitude] = @devicelatitude

IF NOT EXISTS (SELECT 1 FROM [DeviceTypes] WHERE [DeviceType] = @devicetype) INSERT INTO [DeviceTypes] ([DeviceType]) output INSERTED.DeviceTypeId VALUES(@devicetype) ELSE SELECT DeviceTypeId FROM [DeviceTypes] WHERE [DeviceType] = @devicetype

IF NOT EXISTS (SELECT 1 FROM [Students] WHERE [StudentName] = @student) INSERT INTO [Students] ([StudentName]) output INSERTED.StudentId VALUES(@student) ELSE SELECT StudentId FROM [Students] WHERE [StudentName] = @student

IF NOT EXISTS (SELECT 1 FROM [Devices] WHERE [MacAdress] = @macadress) INSERT INTO [Devices] ([MacAdress], [DeviceTypeId], [LocationId], [StudentId]) output INSERTED.DeviceId VALUES(@macadress, @devicetypeid, @devicelocationid, @studentid) ELSE SELECT DeviceId FROM [Devices] WHERE [MacAdress] = @macadress

INSERT INTO [Messages] ([DeviceId], [Temperature], [Humidity], [Created]) VALUES (@deviceid, @temperature, @humidity, @created)