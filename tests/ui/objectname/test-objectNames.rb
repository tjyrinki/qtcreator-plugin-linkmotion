#!/usr/bin/ruby
require 'minitest/autorun'
require 'tdriver'
include TDriverVerify

APPNAME = ARGV[0]

class TestHelloWorld < MiniTest::Test
  def setup
    @sut = TDriver.sut(:sut_qt)
    @app = @sut.run( :name => "#{APPNAME}", :arguments => "-testability", :sleeptime => 2 )
    @mainWindow = @app.child( :name => "MainWindow" )
    verify(1, "MainWindow was found") { @mainWindow }
  end

  def test_smoke
    # go thru all children of the mainWindow and see
    # that every object has a objectName value set.
    children = @mainWindow.children({})
    missingObjectNameCounter = 0
    newObjectNamesCounter = 0
    newObjectNamesResult = Array.new
    expectedObjectNames = open("test-objectNames.lst").read.split(/\r?\n/)
    foundObjectNames = Array.new
    
    # iterate children and store objectName's
    children.each do |child|
      if child['objectName'] == ''
        missingObjectNameCounter += 1
      else
        foundObjectNames.push(child['objectName'])
      end
    end

    # show found objectName's and notify if there were new ones.
    puts
    puts "Found #{foundObjectNames.length} objectNames:"
    foundObjectNames.each do |objName|
      puts " #{objName}"
      unless expectedObjectNames.include? objName
        newObjectNamesCounter += 1
        newObjectNamesResult.push objName
      end
    end

    # show results
    missingExpectedObjectNamesResult = Array.new
    missingExpectedObjectNamesCounter = 0
    expectedObjectNames.each do |objName|
      result = foundObjectNames.include? objName
      #verify_equal(true,1,"The expected objectName '#{objName}' was found.") { result }
      if not result
        missingExpectedObjectNamesCounter += 1
        missingExpectedObjectNamesResult.push objName
      end
    end

    puts
    if missingExpectedObjectNamesCounter == 0
      puts "All expected objectName's were found."
    else
      puts "Failed to find following objectName's:"
      missingExpectedObjectNamesResult.each do |rowData|
        puts " #{rowData}"
      end
    end

    puts
    if newObjectNamesCounter == 0
      puts "No new objectName's were found."
    else
      puts "Found new objectName's:"
      newObjectNamesResult.each do |rowData|
        puts " #{rowData}"
      end
    end
    puts
    puts "There were #{children.length} items with #{missingObjectNameCounter} missing objectName's."
    puts
    
    verify_equal(0,1,"All expected objectName's were found and no new ones.") {
      missingExpectedObjectNamesCounter+newObjectNamesCounter
    }
  end

  def teardown
    @app.kill
  end
end
