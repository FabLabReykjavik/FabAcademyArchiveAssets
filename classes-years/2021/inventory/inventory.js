var sum = 0
var symbol = '?'
function total() {
   document.write("<b>total:</b> "+symbol+sum.toFixed(2))
   }
function table(data) {
   for (key in data) {
      var topic = data[key]
      document.write("<b><font size='+1'><i>"+topic["name"]+"</i></font></b><br>")
      document.write("<table border=1>")
      document.write("<tr>")
      document.write("<th>quantity</th>")
      document.write("<th>item</th>")
      document.write("<th>description</th>")
      document.write("<th>list unit price</th>")
      document.write("<th>extended price</th>")
      document.write("</tr>")
      for (key in topic["vendors"]) {
         var vendor = topic["vendors"][key]
         document.write("<tr align='center'><td><b><a href="+vendor["URL"]+">"+vendor["name"]+"</a></b></td></tr>")
         var vendortotal = 0
         var currency = vendor["currency"]
         if (currency == "USD")
            symbol = '$'
         for (key in vendor["categories"]) {
            var category = vendor["categories"][key]
            document.write("<tr align='center'><td></td><td><b><i>"+category["name"]+"</i></b></td></tr>")
            for (key in category["items"]) {
               var item = category["items"][key]
               document.write("<tr>")
               document.write("<td>"+item["quantity"]+"</td>")
               document.write("<td><a href="+item["URL"]+">"+item["item"]+"</a></td>")
               document.write("<td>"+item["description"]+"</td>")
               document.write("<td align='right'>"+symbol+parseFloat(item["price"]).toFixed(2)+"</td>")
               var extended = parseFloat(item["quantity"])*parseFloat(item["price"])
               document.write("<td align='right'>"+symbol+extended.toFixed(2)+"</td>")
               vendortotal += extended
               document.write("</tr>")
               }
            }
         document.write("<tr align='right'><td></td><td></td><td></td><td><b>total</b></td><td>"+symbol+vendortotal.toFixed(2)+"</td></tr>")
         sum += vendortotal
         }
      document.write("</table>")
      }
   }
