=begin 
Ruby (First PlugIn.rb)코드입니다.
=end
require 'pathname'

def GetInfo ()
	szName = "첫번째 플러그인"
	szDesc = "나의 첫번째 플러그인입니다"
	szVersion = "1.0.0"
	szDate = "2016.04.22"
	szAuthor = "박창진"
	szEmail = "crazevil@gmail.com"
	return [szName, szDesc, szVersion, szDate, szAuthor, szEmail]
end

def Rename (szSrcFile) 
	szFolder =  File.split(szSrcFile)[0]
	szFile =  File.split(szSrcFile)[1]
	
	nIndex = szFile.rindex(".")
	szExt = szFile.slice!(nIndex, szFile.length - nIndex)

	szNumber = szFile.match(/\d+/)[0]

	return szFolder  + "\\" + szNumber + szExt
end
