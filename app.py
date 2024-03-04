from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

# Counter to force page refresh
refresh_counter = 0

# Initial percent value
percent = 0
percent2 = 0

@app.route('/')
def index():
    global refresh_counter
    # Increment the counter on every request
    refresh_counter += 1

    return render_template('base.html', percent=percent, refresh_counter=refresh_counter)

@app.route('/update_fluid')
def update_fluid():
    global percent, percent2, refresh_counter

    percent_from_url = int(request.args.get('percent', percent))
    percent2_from_url = int(request.args.get('percent2', percent2))

    percent_from_url = max(0, min(percent_from_url, 100))
    percent2_from_url = max(0, min(percent2_from_url, 100))

    percent = percent_from_url
    percent2 = percent2_from_url

    refresh_counter += 1

    return jsonify({'refresh_counter': refresh_counter, 'percent': percent, 'percent2': percent2})


@app.route('/get_percent')
def get_percent():
    global refresh_counter, percent
    # Increment the counter on every request
    refresh_counter += 1

    # Simulate updating the percent value (replace this with your actual logic)
    print(percent)

    # Return the updated counter and percent value as JSON
    return jsonify({'refresh_counter': refresh_counter, 'percent': percent})

if __name__ == '__main__':
    app.run(debug=True)
