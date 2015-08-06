extern crate mio;

use mio::{Handler, EventLoop, EventSet, Token};
use super::ssl_handler::SSLHandler;

pub struct SSLLayer<T: SSLHandler>{
    handler: T
}

impl <T: SSLHandler> SSLLayer<T> {
    pub fn new(handler: T) -> SSLLayer<T> {
        SSLLayer::<T> {
            handler: handler
        }
    }
}

impl <T: SSLHandler> Handler for SSLLayer<T> {
    type Timeout = usize;
    type Message = ();
    
    fn ready(&mut self, event_loop: &mut EventLoop<SSLLayer<T>>, token: Token, events: EventSet) {
        println!("SSL READY");
        self.handler.ready(event_loop, token, events);
    }
}
