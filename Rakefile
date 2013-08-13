require "bundler/gem_tasks"
require 'rake/testtask'
require 'rake/extensiontask'

Rake::TestTask.new do |t|
  t.test_files = FileList['test/*_test.rb']
end

Rake::ExtensionTask.new('edgecast_token_extension')
Rake::Task[:test].prerequisites << :compile

desc "Run tests"
task :default => :test
