# Rust

## 安装rust
```
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs
less sh.rustup.sh
sh ./sh.rustup.rs
```

## Rust 的包管理和构建系统
```
# cargo install 
apt install cargo

# cargo 管理与构建项目
mkdir project && cd project
cargo init
cargo build
cargo run 
```

## Crates用于库的分发和跟踪
crates.io 是一个很好的社区 crate 注册网站。
把一个 crate 添加到你的 Rust 项目，首先要在 Cargo.toml 文件中添加这个 crate。例如，要安装随机数函数，我使用名为 rand 的 crate，使用 * 作为通配符，以确保在编译时获得最新版本：
```
[package]
name = "myproject"
version = "0.1.0"
authors = ["Seth <seth@opensource.com>"]
edition = "2022"

[dependencies]
rand = "*"
```
代码中最顶行使用use语句
```
use rand::Rng;
```

## 结构体使用
```rust
struct Penguin {
  genus: String,
  species: String,
  extinct: bool,
  classified: u64,
}

fn main() {
  let p = Penguin { genus: "Pygoscelis".to_owned(),
    species: "R adeliæ".to_owned(), 
    extinct: false, 
    classified: 1841 };
  printer(p);
}

fn printer(p: Penguin) {
  println!("Species: {}", p.species);    
  println!("Genus: {}", p.genus);
  println!("Classified in {}", p.classified);
  if p.extinct == true {
    println!("Sadly this penguin has been made extinct.");
  }
}
```
