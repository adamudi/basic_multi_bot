extern crate mio;

use mio::{EventLoop, EventSet, Token};
use super::ssl_layer::SSLLayer;

#[allow(unused_variables)]
pub trait SSLHandler {
    fn ready(&mut self, event_loop: &mut EventLoop<SSLLayer<Self>>, token: Token, events: EventSet) where Self: Sized {
    }
}
