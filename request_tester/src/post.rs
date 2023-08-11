use fastcmp::Compare;

pub async fn test_post(client: &reqwest::Client, filename: String) -> Result<(), &'static str> {
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