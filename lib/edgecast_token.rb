require 'edgecast_token_extension'

class EdgecastToken
  def self.generate(key, input)
    input = "ec_secure=%03d&%s" % [input.length + 14, input]
    encrypt(key, input)
  end
end
