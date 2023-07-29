use std::{
	io::{
		Read,
		Result,
		Error,
		ErrorKind
	},
	time::Instant,
	cmp
};

pub struct UploadData {
	pub total_data_size: u64,
	pub timeout_in_ms: u128,
	pub start_time: Instant,
	pub current_size: u64
}

impl UploadData {
	pub fn new(n: u64, timeout_ms: u128) -> UploadData {
		UploadData { total_data_size: n, timeout_in_ms: timeout_ms, start_time: Instant::now(), current_size: 0 }
	}
}

impl Read for UploadData {
	fn read(&mut self, buf: &mut [u8]) -> Result<usize> {
		let elapsed = self.start_time.elapsed();
		let elapsed_in_ms = elapsed.as_millis();

		if elapsed_in_ms >= self.timeout_in_ms {
			return Err(Error::new(ErrorKind::Other, "Error sending upload data: Timeout"));
		}

		let const_buf_size: u64 = 8 * 1024;
		let mut buf_size = buf.len() as u64;
		buf_size = cmp::min(buf_size, const_buf_size);

		let data_to_send = self.total_data_size - self.current_size;
		Ok(0)
	}
}

use reqwest;

#[tokio::main]
async fn main() -> std::result::Result<(), Box<dyn std::error::Error>> {
	let chunks: Vec<std::result::Result<_, std::io::Error>> = vec![Ok("Hello"), Ok(""),  Ok("World"), Ok("!")];
	let stream = futures_util::stream::iter(chunks);

	let body = reqwest::Body::wrap_stream(stream);

	let client = reqwest::Client::new();

	let res = client.post("http://localhost:8080")
		.body(body)
		.send()
		.await?;

	println!("{:?}", res);

	Ok(())
}
