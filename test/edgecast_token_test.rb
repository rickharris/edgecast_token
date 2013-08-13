require 'test/unit'
require 'edgecast_token'

class EdgecastTokenTest < Test::Unit::TestCase
  def test_correct_token_no_args
    key = "supercoolencryptionkey"
    input = ""
    assert_equal "562035ba986e43f8033b58eecad3",
      EdgecastToken.generate(key, input)
  end

  def test_correct_token_with_args
    key = "supercoolencryptionkey"
    input = "ec_expire=123456789"
    assert_equal "562035ba986e43f8033b58eccdd3b66415caed4be140ef54bc465d36e744064b5c",
      EdgecastToken.generate(key, input)
  end
end
