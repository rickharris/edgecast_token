# edgecast_token
### EdgeCast token generator for Ruby [![Build Status](https://travis-ci.org/rickharris/edgecast_token.png?branch=master)](https://travis-ci.org/rickharris/edgecast_token)

This library generates tokens to be used for EdgeCast's token auth feature.

### Usage

```ruby
EdgecastToken.generate("my super secret key", "ec_expire=#{Time.now + 2.days}")
```

### Installation via git (for now)

```ruby
gem 'edgecast_token', github: 'rickharris/edgecast_token'
```
