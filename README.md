# edgecast_token
### EdgeCast token generator for Ruby [![Build Status](https://travis-ci.org/rickharris/edgecast_token.png?branch=master)](https://travis-ci.org/rickharris/edgecast_token)

This library generates tokens to be used for EdgeCast's token auth feature.

### Usage

```ruby
EdgecastToken.generate("my super secret key", "ec_expire=#{Time.now + 2.days}")
```

### Installation

In your Gemfile

```ruby
gem 'edgecast_token'
```

or on the command line

```sh
gem install edgecast_token
```
