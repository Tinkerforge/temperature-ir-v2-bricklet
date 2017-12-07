Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Temperature IR Bricklet 2.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim tir As New BrickletTemperatureIRV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current ambient temperature (unit is °C/10)
        Dim ambientTemperature As Short = tir.GetAmbientTemperature()
        Console.WriteLine("Ambient Temperature: " + (ambientTemperature/10.0).ToString() + " °C")

        ' Get current object temperature (unit is °C/10)
        Dim objectTemperature As Short = tir.GetObjectTemperature()
        Console.WriteLine("Object Temperature: " + (objectTemperature/10.0).ToString() + " °C")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
