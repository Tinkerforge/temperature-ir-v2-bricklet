Imports System
Imports Tinkerforge

Module ExampleWaterBoiling
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Temperature IR Bricklet 2.0

    ' Callback subroutine for object temperature reached callback
    Sub ObjectTemperatureCB(ByVal sender As BrickletTemperatureIRV2, _
                            ByVal temperature As Short)
        Console.WriteLine("Object Temperature: " + (temperature/10.0).ToString() + " °C")
        Console.WriteLine("The water is boiling!")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim tir As New BrickletTemperatureIRV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
        tir.SetEmissivity(64224)

        ' Register object temperature reached callback to subroutine ObjectTemperatureCB
        AddHandler tir.ObjectTemperatureCallback, AddressOf ObjectTemperatureCB

        ' Configure threshold for object temperature "greater than 100 °C"
        ' with a debounce period of 10s (10000ms)
        tir.SetObjectTemperatureCallbackConfiguration(10000, False, ">"C, 100*10, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
