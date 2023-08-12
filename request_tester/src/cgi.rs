use fastcmp::Compare;

pub async fn test_cgi(client: &reqwest::Client, filename: String) -> Result<(), &'static str> {
	let res = client.get("http://localhost:8080/scripts/test.pl").send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 200 {
				return Err("Got wrong Status code")
			}
			match ret.text().await {
				Ok(text) => {
					if text != "Hello World\n" {
						return Err("Got wrong body")
					}
				},
				Err(_) => return Err("Could not parse body")
			}
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/scripts/test.py?first_name=Hello&last_name=World").send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 200 {
				return Err("Got wrong Status code")
			}
			match ret.text().await {
				Ok(text) => {
					if text != "First Name: Hello, Last Name: World\n" {
						return Err("Got wrong body")
					}
				},
				Err(_) => return Err("Could not parse body")
			}
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/scripts/custom_status.py").send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 505 {
				return Err("Expected status 505")
			}
		},
		Err(_) => return Err("Could not parse body")
	}

	let res = client.post("http://localhost:8080/scripts/post_echo.py")
		.header("Content-Type", "text/plain")
		.body("This is a test")
		.send()
		.await;
	match res {
		Ok(ret) => {
			if ret.status() != 200 {
				return Err("Got wrong Status code")
			}
			match ret.text().await {
				Ok(text) => {
					if text != "This is a test\n" {
						return Err("Got wrong body")
					}
				},
				Err(_) => return Err("Could not parse body")
			}
		},
		Err(_) => return Err("Failed to send request")
	}

	let chunks: Vec<std::result::Result<_, std::io::Error>> = vec![Ok("Another"), Ok(" "),  Ok("Test"), Ok("!")];
	let stream = futures_util::stream::iter(chunks);

	let body = reqwest::Body::wrap_stream(stream);

	let res = client.post("http://localhost:8080/scripts/post_echo.py")
		.header("Content-Type", "text/plain")
		.body(body)
		.send()
		.await;
	match res {
		Ok(ret) => {
			if ret.status() != 200 {
				return Err("Got wrong Status code")
			}
			match ret.text().await {
				Ok(text) => {
					if text != "Another Test!\n" {
						return Err("Got wrong body")
					}
				},
				Err(_) => return Err("Could not parse body")
			}
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/scripts/not_executable.py").send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 500 {
				return Err("Expected status 500")
			}
		},
		Err(_) => return Err("Could not parse body")
	}

	let res = client.get("http://localhost:8080/scripts/crash.py").send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 500 {
				return Err("Expected status 500")
			}
		},
		Err(_) => return Err("Could not parse body")
	}
	let file = match tokio::fs::read("test").await {
		Ok(f) => f,
		Err(_) => {
			return Err("Failed to open file")
		}
	};

	let part = reqwest::multipart::Part::bytes(file.clone())
		.file_name(filename.clone())
		.mime_str("text/plain");
	let part = match part {
		Ok(part) => part,
		Err(_) => return Err("Failed to convert file")
	};

	let form = reqwest::multipart::Form::new()
		.part("myFile", part);

	let res = client.post("http://localhost:8080/scripts/post.py")
		.multipart(form)
		.send()
		.await;
	match res {
		Ok(ret) => if ret.status() != 201 {
			println!("{:?}", ret);
			return Err("Failed to upload file")
		},
		Err(err) => {
			println!("{:?}", err);
			return Err("Failed to send request")
		}
	}

	let res = client.get(format!("http://localhost:8080/scripts/{}", filename)).send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 200 {
				return Err("Could not retrive uploaded file again")
			} else {
				let content = match ret.bytes().await {
					Ok(bytes) => bytes,
					Err(_) => return Err("Could not retrive bytes")
				}.to_vec();
				if !content.feq(&file) {
					return Err("File was uploaded incorrect")
				 }
			}
		},
		Err(_) => {
			return Err("Failed to send request")
		}
	}

	Ok(())
}
