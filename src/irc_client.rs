use std::net::TcpStream;
use std::io::{Write, Read};
use std::io;
use std::str;

enum ConnectedState {
    Handshake
}

pub struct DisconnectedIrcClient<'a> {
    pub host: &'a str,
    pub port: &'a str,
    pub nickname: &'a str,
    pub rooms: Vec<String>,
}

pub struct ConnectedIrcClient<'a> {
    pub disconnected: &'a DisconnectedIrcClient<'a>,
    pub connection: TcpStream,
    state: ConnectedState,
    pub buffer: [u8; 500],
}

impl<'a> DisconnectedIrcClient<'a> {
    pub fn new(host: &'a str, port: &'a str, nickname: &'a str, rooms: Vec<String>) -> DisconnectedIrcClient<'a> {
        DisconnectedIrcClient{host: host, port: port, nickname: nickname, rooms: rooms}
    }
}

impl<'a> ConnectedIrcClient<'a> {
    pub fn new(client: &'a DisconnectedIrcClient<'a>) -> Result<ConnectedIrcClient<'a>, io::Error> {
        let combined_address = format!("{}:{}", client.host, client.port);
        TcpStream::connect(&*combined_address).map(|x| ConnectedIrcClient{disconnected: &client, connection: x, state: ConnectedState::Handshake, buffer: [0; 500]})
    }

    fn read(&mut self) {
        let response = self.connection.read(&mut self.buffer).map(|bytes_read| str::from_utf8(&self.buffer[0..bytes_read]).unwrap_or("Invalid UTF-8 sequence")).unwrap_or("Empty Response");
        println!("{}", response);
    }

    pub fn tick(&mut self) {
        match &self.state {
            Handshake => {
                let _ = write!(self.connection, "NICK :{}\n", self.disconnected.nickname);
                let _ = write!(self.connection, "USER {} 0 0 :{}\n", self.disconnected.nickname, self.disconnected.nickname);
            }
        }
        loop {
            self.read();
        }
        ()
    }
}
// sock.write("NICK :" + nickname + "\n");
// sock.write("USER " + nickname + " 0 0 :" + nickname + "\n");
