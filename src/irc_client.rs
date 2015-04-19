pub struct IrcClient {
    pub host: &'static str,
    pub port: &'static str,
    pub nickname: &'static str,
    pub rooms: Vec<String>,
}

impl IrcClient {
    pub fn connect(&self) {
        
    }
}
