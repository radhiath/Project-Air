// ====================
// Header kolom untuk Google Sheets
// ====================
const header = ["Waktu", "Score", "Level", "Kondisi", "Kekeruhan (NTU)"];

// ====================
// Fungsi utama menerima data dari ESP
// ====================
function doPost(e) {
  let data;

  // Coba parsing JSON
  try {
    data = JSON.parse(e.postData.contents);
  } catch (err) {
    return ContentService
      .createTextOutput(JSON.stringify({ status: "error", message: "Format JSON salah" }))
      .setMimeType(ContentService.MimeType.JSON);
  }

  // Validasi input
  let validationResult = validateInput(data);
  if (validationResult) {
    Logger.log("Validasi input gagal");
    return validationResult;
  }
  Logger.log("Validasi input berhasil");

  // Nama sheet berdasarkan tanggal
  const today = new Date();
  const dateString = Utilities.formatDate(today, Session.getScriptTimeZone(), "dd-MM-yyyy");

  // Cek sheet sudah ada atau belum
  let sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName(dateString);
  if (!sheet) {
    sheet = SpreadsheetApp.getActiveSpreadsheet().insertSheet(dateString);
    sheet.appendRow(header);
  }

  sheet.setFrozenRows(1);

  // Siapkan data untuk disimpan
  let dataRow = [
    data.time,
    data.score,
    data.level,
    data.condition,
    data.ntu //
  ];

  // Simpan data ke sheet
  sheet.appendRow(dataRow);

  return ContentService
    .createTextOutput(JSON.stringify({ status: "success", message: "Data berhasil diproses" }))
    .setMimeType(ContentService.MimeType.JSON);
}


// ====================
// Fungsi utama buat server ngefetch
// ====================
function doGet(e) {
  if (e.parameter.sheet_list) {
    const sheets = SpreadsheetApp.getActiveSpreadsheet().getSheets().map(s => s.getName());
    return ContentService.createTextOutput(JSON.stringify({ sheets }))
      .setMimeType(ContentService.MimeType.JSON);
  }

  const sheetName = e.parameter.sheet || SpreadsheetApp.getActiveSpreadsheet().getSheets()[0].getName();
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName(sheetName);

  if (!sheet) {
    return ContentService.createTextOutput(JSON.stringify({ error: "Sheet tidak ditemukan" }))
      .setMimeType(ContentService.MimeType.JSON);
  }

  const data = sheet.getDataRange().getDisplayValues(); // Ambil semua dalam format tampilan
  const headers = data.shift();
  const result = data.map(row => {
    let obj = {};
    headers.forEach((key, index) => {
      if (key !== "") obj[key] = row[index];
    });
    return obj;
  });

  return ContentService.createTextOutput(JSON.stringify(result))
    .setMimeType(ContentService.MimeType.JSON);
}

function validateInput(data) {
  // Validasi input data
  if (data.time == null || data.score == null || data.level == null || data.condition == null || 
      data.ntu == null) {
    return ContentService.createTextOutput("Data tidak lengkap");
  }
  return null;
}

function roundThreeDecimal(number) {
  if (number < 0.001) return 0.001; 
  return Math.round(number * 1000) / 1000;
}
