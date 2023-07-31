use reqwest;

#[tokio::main]
async fn main() -> std::result::Result<(), Box<dyn std::error::Error>> {
	let chunks: Vec<std::result::Result<_, std::io::Error>> = vec![Ok("Hello"), Ok(" "),  Ok("World"), Ok("!")];
	let stream = futures_util::stream::iter(chunks);

	let body = reqwest::Body::wrap_stream(stream);

	let client = reqwest::Client::new();

	let res = client.post("http://localhost:8080")
		.body(body)
		.send()
		.await;

	match res {
		Ok(res) => println!("{:?}", res),
		Err(err) => {
			if err.is_connect() {
				println!("Connection error. Ensure webserv is started");
			} else {
				println!("Got error in response: {}", err);
			}
		}
	}


	Ok(())
}
