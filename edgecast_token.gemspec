Gem::Specification.new do |s|
  s.name        = 'edgecast_token'
  s.version     = '0.0.0'
  s.date        = '2013-06-11'
  s.summary     = "Generate EdgeCast tokens on the fly"
  s.description = "Generate EdgeCast tokens on the fly"
  s.authors     = ["Rick Harris"]
  s.email       = 'rick.c.harris@gmail.com'
  s.files = Dir.glob('lib/**/*.rb') + Dir.glob('ext/**/*.{c,rb}')
  s.extensions = ['ext/edgecast_token_extension/extconf.rb']
  s.homepage    =
    'http://rubygems.org/gems/edgecast_token'

  s.add_development_dependency "rake-compiler"
end
