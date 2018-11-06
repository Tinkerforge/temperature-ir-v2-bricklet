use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, temperature_ir_v2_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Temperature IR Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let temperature_ir_v2_bricklet = TemperatureIRV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
    temperature_ir_v2_bricklet.set_emissivity(64224);

    //Create listener for object temperature reached events.
    let object_temperature_listener = temperature_ir_v2_bricklet.get_object_temperature_receiver();
    // Spawn thread to handle received events. This thread ends when the temperature_ir_v2_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in object_temperature_listener {
            println!("Object Temperature: {}{}", event as f32 / 10.0, " °C");
            println!("The water is boiling!");
        }
    });

    // Configure threshold for object temperature "greater than 100 °C"
    // with a debounce period of 10s (10000ms)
    temperature_ir_v2_bricklet.set_object_temperature_callback_configuration(10000, false, '>', 100 * 10, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
