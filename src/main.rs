use std::thread;

use irc_client::{ConnectedIrcClient, DisconnectedIrcClient};
mod irc_client;

fn main() {
    let irc = DisconnectedIrcClient::new("localhost", "6667", "talexander", vec!["#dragonflybsd".to_string()]);
    let mut connected_irc = ConnectedIrcClient::new(&irc).unwrap();
    loop {
        connected_irc.tick();
        thread::sleep_ms(1000);
    }
}
