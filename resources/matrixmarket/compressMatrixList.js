//Requires LZW.js to be loaded
//takes a js object containing the matrix list as argument
//converts that object to a json string
//compresses that string with LZW
//displays the compressed string in a new window for easier saving to file
//once the data is saved to a file, it can be included by placing <script src="compressedMatrixList_basic.json.js"></script> into the page head
//the compressed data is saved in a variable called: compressedMatrixList
//to get the matrixList js object back: decompress compressedMatrixList with LZW.decompress() and parse it with JSON.parse()
function printCompressedMatrixList(matrixListObject){
	var compressed = LZW.compress(JSON.stringify(matrixListObject));
	var url = 'data:text/json;charset=utf8,';
	url += encodeURIComponent('var compressedMatrixList = ['+ compressed + ']');
	window.open(url, '_blank');
	window.focus();
}