use reqwest;
use color_print::cprintln;
use random_string::generate;
use fastcmp::Compare;

#[tokio::main]
async fn main() -> std::result::Result<(), Box<dyn std::error::Error>> {
	let chunks: Vec<std::result::Result<_, std::io::Error>> = vec![Ok(""), Ok(" "),  Ok("World"), Ok("!")];
	let stream = futures_util::stream::iter(chunks);

	let _body = reqwest::Body::wrap_stream(stream);

	let client = reqwest::Client::new();
	let upload_name = generate(5, "0123456789");
	evaluate_test("Get", test_get(&client).await);
	evaluate_test("Post", test_post(&client, upload_name.clone()).await);
	evaluate_test("Delete", test_delete(&client, upload_name).await);

	Ok(())
}

fn evaluate_test(test_name: &str, test_output: Result<(), &str>) {
	print!("{} Test: ", test_name);
	match test_output {
		Ok(()) => cprintln!("<green>passed</green>"),
		Err(err) => cprintln!("<red>failed. {}</red>", err)
	}
}

async fn test_get(client: &reqwest::Client) -> Result<(), &'static str> {
	let res = client.get("http://localhost:8080/").send().await;
	if let Ok(ret) = res {
		if ret.status() != 200 {
			return Err("Got wrong status for /");
		}
	} else {
		return Err("Failed to send request");
	}

	let res = client.get("http://localhost:8080/asfasd").send().await;
	match res {
		Ok(ret) => if ret.status() != 404 {
			return Err("Got wrong status for random garbage")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/dir?test=test").send().await;
	match res {
		Ok(ret) => if ret.status() != 200 {
			return Err("Failed to separate query string")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/dir/garbage").send().await;
	match res {
		Ok(ret) => if ret.status() != 404 {
			return Err("Got wrong status for non existing file")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/dir/another/test").send().await;
	match res {
		Ok(ret) => if ret.status() != 200 {
			return Err("Failed to get nested directory")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8081/hello").send().await;
	match res {
		Ok(ret) => if ret.status() != 200 {
			return Err("Failed to get from another server")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/hi").send().await;
	match res {
		Ok(ret) => if ret.status() != 405 {
			return Err("Method should not be allowed")
		},
		Err(_) => return Err("Failed to send request")
	}

	Ok(())
}

async fn test_post(client: &reqwest::Client, filename: String) -> Result<(), &'static str> {
	let mut file = match tokio::fs::read("test").await {
		Ok(f) => f,
		Err(_) => {
			return Err("Failed to open file")
		}
	};

	let part = reqwest::multipart::Part::bytes(file.clone())
		.mime_str("text/plain");
	let part = match part {
		Ok(part) => part,
		Err(_) => return Err("Failed to convert file")
	};

	let form = reqwest::multipart::Form::new()
		.part(filename.clone(), part);

	let res = client.post("http://localhost:8080/upload")
		.multipart(form)
		.send()
		.await;
	match res {
		Ok(ret) => if ret.status() != 201 {
			return Err("Failed to upload file")
		},
		Err(err) => {
			println!("{:?}", err);
			return Err("Failed to send request")
		}
	}

	let res = client.post("http://localhost:8080/dir")
		.body("Hi")
		.send()
		.await;
	match res {
		Ok(ret) => if ret.status() != 405 {
			return Err("Method should be forbidden")
		},
		Err(err) => {
			println!("{:?}", err);
			return Err("Failed to send request")
		}
	}

	let res = client.get(format!("http://localhost:8080/upload/{}", filename)).send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 200 {
				return Err("Could not retrive uploaded file again")
			} else {
				let content = match ret.bytes().await {
					Ok(bytes) => bytes,
					Err(_) => return Err("Could not retrive bytes")
				}.to_vec();
				file.append(b"\r\n".to_vec().as_mut());
				if !content.feq(&file) {
					return Err("File was uploaded incorrect")
				 }
			}
		},
		Err(_) => {
			return Err("Failed to send request")
		}
	}

	let payload: [u8; 501] = [0; 501];

	let res = client.post("http://localhost:8080/upload")
	.body(payload.to_vec())
	.send()
	.await;
	match res {
		Ok(ret) => if ret.status() != 413 {
			return Err("Content should be too large")
		},
		Err(err) => {
			println!("{:?}", err);
			return Err("Failed to send request")
		}
	}

	Ok(())
}

async fn test_delete(client: &reqwest::Client, filename: String) -> Result<(), &'static str> {
	let res = client.delete(format!("http://localhost:8080/upload/{}", filename)).send().await;
	match res {
		Ok(ret) => if ret.status() != 200 {
			return Err("Could not delete file")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.delete("http://localhost:8080/dir/index.html").send().await;
	match res {
		Ok(ret) => if ret.status() != 405 {
			return Err("Could not delete file")
		},
		Err(_) => return Err("Failed to send request")
	}

	Ok(())
}