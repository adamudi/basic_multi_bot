#![feature(rustc_private)]

mod ssl_layer;

use mio::*;
use mio::tcp::*;
use bytes::{ByteBuf, MutByteBuf, SliceBuf};
use std::io;
use std::io::Read;
use ssl_layer::ssl_handler::SSLHandler;
use ssl_layer::ssl_layer::SSLLayer;

#[macro_use]
extern crate log;
extern crate env_logger;
extern crate mio;
extern crate bytes;

const CLIENT: Token = Token(0);

struct EchoClient {
    sock: TcpStream,
    msgs: Vec<&'static str>,
    tx: SliceBuf<'static>,
    mut_buf: Option<MutByteBuf>,
    token: Token,
    interest: EventSet
}


impl EchoClient {
    fn new(sock: TcpStream, tok: Token,  mut msgs: Vec<&'static str>) -> EchoClient {
        let curr = msgs.remove(0);

        EchoClient {
            sock: sock,
            msgs: msgs,
            tx: SliceBuf::wrap(curr.as_bytes()),
            mut_buf: Some(ByteBuf::mut_with_capacity(2048)),
            token: tok,
            interest: EventSet::none()
        }
    }

    fn readable(&mut self, event_loop: &mut EventLoop<SSLLayer<Echo>>) -> io::Result<()> {
        debug!("client socket readable");

        let mut buf = self.mut_buf.take().unwrap();

        match self.sock.try_read_buf(&mut buf) {
            Ok(None) => {
                panic!("We just got readable, but were unable to read from the socket?");
            }
            Ok(Some(r)) => {
                debug!("CLIENT : We read {} bytes!", r);
            }
            Err(e) => {
                panic!("not implemented; client err={:?}", e);
            }
        };

        // prepare for reading
        let mut buf = buf.flip();

        let mut content: String = String::with_capacity(10);

        match buf.read_to_string(&mut content) {
            Ok(i) => println!("Stringed {:?} bytes", i),
            Err(_) => panic!("I give up."),
        };

        self.mut_buf = Some(buf.flip());

        println!("{:?}", content);

        self.interest.remove(EventSet::readable());

        self.next_msg(event_loop).unwrap();
        
        event_loop.reregister(&self.sock, self.token, self.interest, PollOpt::edge() | PollOpt::oneshot())
    }

    fn writable(&mut self, event_loop: &mut EventLoop<SSLLayer<Echo>>) -> io::Result<()> {
        debug!("client socket writable");

        match self.sock.try_write_buf(&mut self.tx) {
            Ok(None) => {
                debug!("client flushing buf; WOULDBLOCK");
                self.interest.insert(EventSet::writable());
            }
            Ok(Some(r)) => {
                debug!("CLIENT : we wrote {} bytes!", r);
                self.interest.insert(EventSet::readable());
                self.interest.remove(EventSet::writable());
            }
            Err(e) => debug!("not implemented; client err={:?}", e)
        }

        event_loop.reregister(&self.sock, self.token, self.interest, PollOpt::edge() | PollOpt::oneshot())
    }

    fn next_msg(&mut self, event_loop: &mut EventLoop<SSLLayer<Echo>>) -> io::Result<()> {
        if self.msgs.is_empty() {
            event_loop.shutdown();
            return Ok(());
        }

        let curr = self.msgs.remove(0);

        debug!("client prepping next message");
        self.tx = SliceBuf::wrap(curr.as_bytes());

        self.interest.insert(EventSet::writable());
        event_loop.reregister(&self.sock, self.token, self.interest, PollOpt::edge() | PollOpt::oneshot())
    }
}

struct Echo {
    client: EchoClient,
}

impl Echo {
    fn new(client: TcpStream, msgs: Vec<&'static str>) -> Echo {
        Echo {
            client: EchoClient::new(client, CLIENT, msgs)
        }
    }
}

impl SSLHandler for Echo {
    fn ready(&mut self, event_loop: &mut EventLoop<SSLLayer<Echo>>, token: Token, events: EventSet) {
        if events.is_readable() {
            match token {
                CLIENT => self.client.readable(event_loop).unwrap(),
                i => println!("Found unexpected token: {:?}", i)
            }
        }

        if events.is_writable() {
            match token {
                CLIENT => self.client.writable(event_loop).unwrap(),
                i => println!("Non-client writable: {:?}", i)
            };
        }
    }
}

pub fn main() {
    env_logger::init().unwrap();
    
    debug!("Starting TEST_ECHO_SERVER");
    let mut event_loop = EventLoop::new().unwrap();

    let address = std::str::FromStr::from_str("173.194.123.37:80").unwrap();
    let (socket, _) = TcpSocket::v4().unwrap().connect(&address).unwrap();

    // Connect to the server
    event_loop.register_opt(&socket, CLIENT, EventSet::writable(), PollOpt::edge() | PollOpt::oneshot()).unwrap();

    // Start the event loop
    let final_handler = Echo::new(socket, vec!["GET / HTTP/1.0\r\nHost: google.com\r\n\r\n"]);
    let mut ssl_handler = SSLLayer::new(final_handler);
    event_loop.run(&mut ssl_handler).unwrap();
}
