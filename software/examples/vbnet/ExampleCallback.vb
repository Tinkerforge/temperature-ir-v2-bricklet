Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Temperature IR Bricklet 2.0

    ' Callback subroutine for object temperature callback (parameter has unit °C/10)
    Sub ObjectTemperatureCB(ByVal sender As BrickletTemperatureIRV2, _
                            ByVal temperature As Short)
        Console.WriteLine("Object Temperature: " + (temperature/10.0).ToString() + " °C")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim tir As New BrickletTemperatureIRV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register object temperature callback to subroutine ObjectTemperatureCB
        AddHandler tir.ObjectTemperatureCallback, AddressOf ObjectTemperatureCB

        ' Set period for object_temperature callback to 1s (1000ms) without a threshold
        tir.SetObjectTemperatureCallbackConfiguration(1000, False, "x"C, 0, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
