use std::net::TcpStream;
use std::io::Write;
use std::io::Read;
use std::str;

use irc_client::IrcClient;
mod irc_client;

fn main() {
    let irc = IrcClient{host: "irc.prison.net", port: "6667", nickname: "talexander", rooms: vec!["#dragonflybsd".to_string()]};
    
    let mut connection = TcpStream::connect("google.com:80").ok().expect("failed to connect :-(");
    let _ = connection.write(b"GET / HTTP/1.1\r\nUser-Agent: curl/7.37.0\r\nHost: google.com\r\n\r\n");
    let mut buf: [u8; 500] = [0; 500];
    loop {
        match connection.read(&mut buf).ok() {
            None => println!("Empty Response"),
            Some(bytes_read) => {
                println!("{}", str::from_utf8(&buf[0..bytes_read]).ok().expect("Invalid UTF-8 sequence"));
            }
        }
    }
}
