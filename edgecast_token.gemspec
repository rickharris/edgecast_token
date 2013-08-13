# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'edgecast_token/version'

Gem::Specification.new do |spec|
  spec.name          = "edgecast_token"
  spec.version       = EdgecastToken::VERSION
  spec.authors       = ["Rick Harris"]
  spec.email         = ["rick.c.harris@gmail.com"]
  spec.description   = %q{Generate EdgeCast access tokens in Ruby using EdgeCast's encryption algorithm in C.}
  spec.summary       = %q{Generate EdgeCast access tokens in Ruby}
  spec.homepage      = "https://github.com/rickharris/edgecast_token"
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.3"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"
end
